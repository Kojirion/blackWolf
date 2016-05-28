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


bool Controller::requestMove(const Move& move)
{
    if (!game.userTurn()) {
        //premove
        if (canvas.getColorOn(move.square_1) == game.getUserColor()) {
            premove = move;
            premoveOn = true;
            canvas.setPremove(move);
        }
        return false;
    }

    client.makeMove(move);
    return true;
}

Controller::Controller(sf::Window &theWindow, sfg::Desktop &theDesktop, CallbackSystem &callbackSystem):
    desktop(theDesktop),
    boardWindow(sfg::Window::Create(sfg::Window::BACKGROUND)),
    settingsButton(sfg::Button::Create("Settings")),
    canvas(theWindow),
    settingsWindow(desktop),
    premove({{0,0},{0,0}}), premoveOn(false),
    netWindow(m_currentEvent),
    player1(sfg::Label::Create()),
    player2(sfg::Label::Create())
{
    boardWindow->SetRequisition(static_cast<sf::Vector2f>(theWindow.getSize()));

    canvas.requestMove.connect(boost::bind(&Controller::requestMove, this,_1));
    settingsWindow.settingsDone.connect([this](const PieceToTexPos& pieceToTexPos){
        settingsWindow.enable(false);
        canvas.setPieceColors(pieceToTexPos);
    });
    client.textReady.connect(boost::bind(&NetWidgets::addLine, &netWindow, _1));
    netWindow.sendText.connect(boost::bind(&Client::toClient, &client, _1));

    ButtonBox buttons;
    buttons.flip->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Canvas::flipBoard, &canvas));
    buttons.settings->GetSignal(sfg::Button::OnLeftClick).Connect([]{
       //settingsWindow.enable(true);
    });
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
    mainLayout->Attach(canvas.getBoardWidget(),{0, 0, 3, 2},sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 10.f, 0.f ));
    mainLayout->Attach(promotionFrame, {0, 2, 1, 1});
    mainLayout->Attach(sideLayout, {3,0, 3, 1});
    mainLayout->Attach(moveList.getView(),{3, 1, 3, 3});
    mainLayout->Attach(buttons.layout,{0,4,6,1});

    desktop.Add(settingsWindow.getWidget());

    sfg::Notebook::Ptr notebook(sfg::Notebook::Create());
    notebook->AppendPage(mainLayout,sfg::Label::Create("Board"));
    notebook->AppendPage(netWindow.getWidget(),sfg::Label::Create("Server"));
    notebook->GetSignal(sfg::Notebook::OnTabChange).Connect([this]{
        netWindow.grabEntryFocus();
    });

    callbackSystem.connect(Action::Tab, [notebook](thor::ActionContext<Action> context){
        if (context.event->key.control){
            if (context.event->key.shift)
                notebook->PreviousPage();
            else
                notebook->NextPage();
        }
    });

    boardWindow->Add(notebook);

    desktop.Add(boardWindow);

    buttons.connect->GetSignal(sfg::Button::OnLeftClick).Connect([this, notebook]{
        client.connect();
        notebook->SetCurrentPage(1);
    });

    messages.connect(Messages::ID::GameState, [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameState*>(&message);
        game.setTime(received->white_time, received->black_time);
        game.startClock();

        if (premoveOn)
        {
            premoveOn = false;
            canvas.clearArrows();
            requestMove(premove);
        }
    });

    messages.connect(Messages::ID::GameStart, [this, promotionQueen, notebook](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::GameStart*>(&message);
        player1->SetText(received->p1);
        player2->SetText(received->p2);
        premoveOn = false;
        promotionQueen->SetActive(true);
        updateClocks();
        notebook->SetCurrentPage(0);
    });

    messages.connect(Messages::ID::GameEnd, [this](const Messages::Message&){
        //auto received = boost::polymorphic_downcast<const EndGameMessage*>(&message);
        premoveOn = false;
        canvas.clearArrows();
    });

    callbackSystem.connect(Action::Scroll, [this](thor::ActionContext<Action> context){
        int delta = context.event->mouseWheelScroll.delta;
        netWindow.scroll(delta);
    });
}

void Controller::update()
{
    client.update();
    canvas.display();
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
