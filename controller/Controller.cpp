#include "Controller.hpp"
#include <cassert>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/ButtonBox.hpp"
#include <SFGUI/Notebook.hpp>
#include <boost/cast.hpp>

void Controller::enableWindow(const bool enable)
{
    if (enable) boardWindow->SetState(sfg::Widget::State::NORMAL);
    else boardWindow->SetState(sfg::Widget::State::INSENSITIVE);
}

void Controller::settingsClicked()
{
    //enableWindow(false);
    //settingsWindow.setTree(resources.getTree());
    //settingsWindow.enable(true);
}

void Controller::settingsDone(const std::string& whitePrefix, const std::string& blackPrefix, const std::string& boardSuffix)
{
    settingsWindow.enable(false);
    enableWindow(true);
}

bool Controller::requestMove(const Move& move)
{
    if (!game.userTurn()) {
        //premove
        if (board.getColorOn(move.square_1) == game.getUserColor()) {
            premove = move;
            premoveOn = true;
            board.setPremove(move);
        }
        return false;
    }

    client.makeMove(move);
    return true;
}

struct GrabFocusOnTabChange{
    GrabFocusOnTabChange(const sfg::Notebook::Ptr& notebook, NetWidgets& netWidgets):
        notebook(notebook),
        previousPage(notebook->GetCurrentPage()),
        netWidgets(netWidgets)
    {

    }

    void operator()(){
        if (notebook->GetCurrentPage() != previousPage)
            netWidgets.grabEntryFocus();
    }

private:
    sfg::Notebook::Ptr notebook;
    sfg::Notebook::IndexType previousPage;
    NetWidgets& netWidgets;
};

Controller::Controller(sf::Window &theWindow, sfg::Desktop &theDesktop, CallbackSystem &callbackSystem):
    desktop(theDesktop),
    boardWindow(sfg::Window::Create(sfg::Window::BACKGROUND)),
    settingsButton(sfg::Button::Create("Settings")),
    board(theWindow),
    settingsWindow(desktop),
    premove({{0,0},{0,0}}), premoveOn(false),
    player1(sfg::Label::Create()),
    player2(sfg::Label::Create())
{
    boardWindow->SetRequisition(static_cast<sf::Vector2f>(theWindow.getSize()));

    board.requestMove.connect(boost::bind(&Controller::requestMove, this,_1));
    settingsWindow.settingsDone.connect(boost::bind(&Controller::settingsDone, this,_1,_2,_3));
    client.textReady.connect(boost::bind(&NetWidgets::addLine, &netWindow, _1));
    netWindow.sendText.connect(boost::bind(&Client::toClient, &client, _1));

    ButtonBox buttons;
    buttons.flip->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Canvas::flipBoard, &board));
    buttons.settings->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Controller::settingsClicked, this));
    buttons.resign->GetSignal(sfg::Button::OnLeftClick).Connect([this]{
        client.toClient("resign");
    });
    buttons.draw->GetSignal(sfg::Button::OnLeftClick).Connect([this]{
        client.toClient("draw");
    });
    buttons.connect->GetSignal(sfg::Button::OnLeftClick).Connect([this]{
        client.connect();
    });


    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(2.f);
    mainLayout->Attach(board.getBoardWidget(),{0, 0, 1, 12},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(counters.getView(), {0, 10, 1, 2});
    mainLayout->Attach(player1, {1,0,1,1});
    mainLayout->Attach(clocks.getWhiteClock(),{1, 1, 1, 1});
    mainLayout->Attach(player2, {1,2,1,1});
    mainLayout->Attach(clocks.getBlackClock(),{1, 3, 1, 1});
    mainLayout->Attach(status.getView(),{1, 4, 1, 1});
    mainLayout->Attach(moveList.getView(),{1, 5, 1, 4});
    mainLayout->Attach(buttons.layout,{0,12,2,2});

    desktop.Add(settingsWindow.getWidget());

    sfg::Notebook::Ptr notebook(sfg::Notebook::Create());
    notebook->AppendPage(mainLayout,sfg::Label::Create("Board"));
    notebook->AppendPage(netWindow.getWidget(),sfg::Label::Create("Server"));
    GrabFocusOnTabChange grabFocusOnTabChange(notebook, netWindow);
    notebook->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(grabFocusOnTabChange);

    boardWindow->Add(notebook);

    desktop.Add(boardWindow);

    messages.connect("gameState", [this](const Message& message){
        const GameStateMessage* received = boost::polymorphic_downcast<const GameStateMessage*>(&message);
        game.setTime(received->white_time, received->black_time);
        game.startClock();

        if (premoveOn)
        {
            premoveOn = false;
            board.clearArrows();
            requestMove(premove);
        }
    });

    messages.connect("newGame", [this](const Message& message){
        const NewGameMessage* received = boost::polymorphic_downcast<const NewGameMessage*>(&message);
        player1->SetText(received->p1);
        player2->SetText(received->p2);
        premoveOn = false;

        updateClocks();
    });

    messages.connect("resized", [this](const Message& message){
        const ResizeMessage* received = boost::polymorphic_downcast<const ResizeMessage*>(&message);
        boardWindow->SetAllocation({boardWindow->GetAbsolutePosition(),
                                    static_cast<sf::Vector2f>(received->window.getSize())});
    });

    messages.connect("endGame", [this](const Message& message){
        //auto received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        premoveOn = false;
        board.clearArrows();
    });

    callbackSystem.connect(Action::Scroll, [this](thor::ActionContext<Action> context){
        int delta = context.event->mouseWheelScroll.delta;
        netWindow.scroll(delta);
    });
}

void Controller::update()
{
    client.update();
    board.display();
    updateClocks();
}

void Controller::updateClocks()
{
    clocks.update(game.getWhiteTime(), game.getBlackTime());
}
