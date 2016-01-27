#include "Controller.hpp"
#include <cassert>
#include <SFGUI/Box.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include "components/ButtonBox.hpp"
#include <SFGUI/Notebook.hpp>
#include <boost/cast.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <SFGUI/Frame.hpp>
#include "../messages/GameEnd.hpp"
#include "../messages/GameState.hpp"
#include "../messages/GameStart.hpp"


void Controller::enableWindow(const bool enable)
{
    if (enable) boardWindow->SetState(sfg::Widget::State::NORMAL);
    else boardWindow->SetState(sfg::Widget::State::INSENSITIVE);
}

void Controller::settingsClicked()
{
    //enableWindow(false);
    //settingsWindow.setTree(resources.getTree());
    settingsWindow.enable(true);
}

void Controller::settingsDone(const PieceToTexPos& pieceToTexPos)
{
    settingsWindow.enable(false);
    enableWindow(true);

    board.setPieceColors(pieceToTexPos);
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
    netWindow(m_currentEvent),
    player1(sfg::Label::Create()),
    player2(sfg::Label::Create())
{
    boardWindow->SetRequisition(static_cast<sf::Vector2f>(theWindow.getSize()));

    board.requestMove.connect(boost::bind(&Controller::requestMove, this,_1));
    settingsWindow.settingsDone.connect(boost::bind(&Controller::settingsDone, this,_1));
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

    auto promotionLayout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto promotionGroup = sfg::RadioButtonGroup::Create();

    auto promotionQueen = sfg::RadioButton::Create("Queen", promotionGroup);
    auto promotionBishop = sfg::RadioButton::Create("Bishop", promotionGroup);
    auto promotionKnight = sfg::RadioButton::Create("Knight", promotionGroup);
    auto promotionRook = sfg::RadioButton::Create("Rook", promotionGroup);

    promotionLayout->Pack(promotionQueen);
    promotionLayout->Pack(promotionBishop);
    promotionLayout->Pack(promotionKnight);
    promotionLayout->Pack(promotionRook);

    promotionQueen->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, promotionQueen]{
        if (promotionQueen->IsActive())
            client.toClient("promote q");
    });
    promotionBishop->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, promotionBishop]{
        if (promotionBishop->IsActive())
            client.toClient("promote b");
    });
    promotionKnight->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, promotionKnight]{
        if (promotionKnight->IsActive())
            client.toClient("promote n");
    });
    promotionRook->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, promotionRook]{
        if (promotionRook->IsActive())
            client.toClient("promote r");
    });

    auto promotionFrame = sfg::Frame::Create("Promotion");
    promotionFrame->Add(promotionLayout);

    auto sideLayout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    sideLayout->Pack(player1);
    sideLayout->Pack(clocks.getWhiteClock());
    sideLayout->Pack(player2);
    sideLayout->Pack(clocks.getBlackClock());
    sideLayout->Pack(status.getView());

    sfg::Table::Ptr mainLayout(sfg::Table::Create());
    mainLayout->SetRowSpacings(2.f);
    mainLayout->SetColumnSpacings(2.f);
    mainLayout->Attach(board.getBoardWidget(),{0, 0, 3, 2},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(promotionFrame, {0, 2, 1, 1});
    mainLayout->Attach(sideLayout, {3,0, 3, 1});
    mainLayout->Attach(moveList.getView(),{3, 1, 3, 3});
    mainLayout->Attach(buttons.layout,{0,4,6,1});

    desktop.Add(settingsWindow.getWidget());

    sfg::Notebook::Ptr notebook(sfg::Notebook::Create());
    notebook->AppendPage(mainLayout,sfg::Label::Create("Board"));
    notebook->AppendPage(netWindow.getWidget(),sfg::Label::Create("Server"));
    GrabFocusOnTabChange grabFocusOnTabChange(notebook, netWindow);
    notebook->GetSignal(sfg::Widget::OnMouseLeftPress).Connect(grabFocusOnTabChange);

    boardWindow->Add(notebook);

    desktop.Add(boardWindow);

    buttons.connect->GetSignal(sfg::Button::OnLeftClick).Connect([this, notebook]{
        client.connect();
        notebook->SetCurrentPage(1);
        netWindow.grabEntryFocus();
    });

    messages.connect("gameState", [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameStateMessage*>(&message);
        game.setTime(received->white_time, received->black_time);
        game.startClock();

        if (premoveOn)
        {
            premoveOn = false;
            board.clearArrows();
            requestMove(premove);
        }
    });

    messages.connect("newGame", [this, promotionQueen, notebook](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::NewGameMessage*>(&message);
        player1->SetText(received->p1);
        player2->SetText(received->p2);
        premoveOn = false;
        promotionQueen->SetActive(true);
        updateClocks();
        notebook->SetCurrentPage(0);
    });

    messages.connect("endGame", [this](const Messages::Message& message){
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

void Controller::setEvent(const sf::Event &event)
{
    m_currentEvent = event;
}

void Controller::updateClocks()
{
    clocks.update(game.getWhiteTime(), game.getBlackTime());
}
