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
#include "../messages/TextReady.hpp"
#include "../messages/TextToClient.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


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
    m_whiteClock(Clock::Create()),
    m_blackClock(Clock::Create()),
    settingsWindow(desktop),
    premove({{0,0},{0,0}}), premoveOn(false),
    netWindow(m_currentEvent),
    player1(sfg::Label::Create()),
    player2(sfg::Label::Create())
{
    namespace pt = boost::property_tree;

    pt::ptree tree;
    pt::read_json("Settings.json", tree);

    auto theme = tree.get("Theme", "blackwolf.theme");
    theDesktop.LoadThemeFromFile(theme);

    auto whiteIndex = tree.get("WhitePieces", 1);
    auto blackIndex = tree.get("BlackPieces", 0);
    canvas.setPieceColors({whiteIndex, blackIndex});


    boardWindow->SetRequisition(static_cast<sf::Vector2f>(theWindow.getSize()));

    canvas.requestMove.connect(boost::bind(&Controller::requestMove, this,_1));
    settingsWindow.settingsDone.connect([this](const PieceToTexPos& pieceToTexPos){
        settingsWindow.enable(false);
        canvas.setPieceColors(pieceToTexPos);
    });
    messages.connect(Messages::ID::TextReady, [this](const Messages::Message& message){
        auto received = boost::polymorphic_downcast<const Messages::TextReady*>(&message);
        netWindow.addLine(received->text);
    });

    ButtonBox buttons;
    buttons.flip->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&Canvas::flipBoard, &canvas));
    buttons.settings->GetSignal(sfg::Button::OnLeftClick).Connect([this]{
        settingsWindow.enable(true);
    });
    buttons.resign->GetSignal(sfg::Button::OnLeftClick).Connect([this]{
        messages.triggerEvent(Messages::TextToClient("resign"));
    });
    buttons.draw->GetSignal(sfg::Button::OnLeftClick).Connect([this]{
        messages.triggerEvent(Messages::TextToClient("draw"));
    });

    auto promotionLayout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    auto promotionGroup = sfg::RadioButtonGroup::Create();

    auto createPromotionButton = [this, promotionGroup, promotionLayout](const std::string& name, char letter){
        auto promotionButton = sfg::RadioButton::Create(name, promotionGroup);
        promotionLayout->Pack(promotionButton);
        promotionButton->GetSignal(sfg::ToggleButton::OnToggle).Connect([this, promotionButton, letter]{
            if (promotionButton->IsActive()){
                std::string promote("promote ");
                promote += letter;
                messages.triggerEvent(Messages::TextToClient(promote));
            }
        });
        return promotionButton;
    };

    auto promotionQueen = createPromotionButton("Queen", 'q');
    createPromotionButton("Bishop", 'b');
    createPromotionButton("Knight", 'n');
    createPromotionButton("Rook", 'r');

    auto promotionFrame = sfg::Frame::Create("Promotion");
    promotionFrame->Add(promotionLayout);

    auto sideLayout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

    sideLayout->Pack(player1);
    sideLayout->Pack(m_whiteClock);
    sideLayout->Pack(player2);
    sideLayout->Pack(m_blackClock);
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
        m_whiteClock->setTimeLeft(received->white_time);
        m_blackClock->setTimeLeft(received->black_time);
        if (received->turnColor == Color::White){
            m_blackClock->stop();
            m_whiteClock->start();
        }else{
            assert(received->turnColor==Color::Black);
            m_whiteClock->stop();
            m_blackClock->start();
        }


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
        m_whiteClock->stop();
        m_blackClock->stop();
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
    //clocks.update(game.getWhiteTime(), game.getBlackTime());
}
