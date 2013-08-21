//#ifndef MSMMANAGER_H
//#define MSMMANAGER_H
//#include <boost/noncopyable.hpp>
//#include <boost/msm/front/state_machine_def.hpp>
//#include <boost/msm/back/state_machine.hpp>
//#include <boost/msm/front/functor_row.hpp>
//#include <boost/msm/front/euml/common.hpp>
//#include <boost/msm/front/euml/operator.hpp>
//#include <boost/msm/front/euml/state_grammar.hpp>
//#include "boardmaster.h"
//#include "settingsstate.h"
//#include <SFGUI/Desktop.hpp>

//namespace {

//namespace msm = boost::msm;
//namespace mpl = boost::mpl;

////events
//struct settingsOpen{};
//struct settingsClose{};

////front end
//struct msmManager : public msm::front::state_machine_def<msmManager>
//{
//    sf::Window& bigWindow;
//    settingsState settingsData;
//    sfg::Desktop& desktop;
//    boardMaster boardData;

//    msmManager(sfg::Desktop& theDesktop, sf::Window& theBigWindow):
//        desktop(theDesktop),
//        bigWindow(theBigWindow),
//        boardData(bigWindow, desktop)
//    {
//        desktop.Add(settingsData.window);
//    }

//    //states
//    struct gameOnEntry
//    {
//        template <class Event,class FSM,class STATE>
//        void operator()(Event const&,FSM& fsm,STATE& )
//        {
//            fsm.boardData.boardWindow->SetState(sfg::Widget::NORMAL);
//        }
//    };
//    struct gameOnExit
//    {
//        template <class Event,class FSM,class STATE>
//        void operator()(Event const&,FSM& fsm,STATE& )
//        {
//            fsm.boardData.boardWindow->SetState(sfg::Widget::INSENSITIVE);
//        }
//    };
//    struct gameOnTag{};
//    typedef msm::front::euml::func_state<gameOnTag,gameOnEntry,gameOnExit> gameOn;

//    struct settingsEntry
//    {
//        template <class Event,class FSM,class STATE>
//        void operator()(Event const&,FSM& fsm,STATE& )
//        {
//            fsm.settingsData.makeVisible();
//            fsm.desktop.BringToFront(fsm.settingsData.window);
//        }
//    };
//    struct settingsExit
//    {
//        template <class Event,class FSM,class STATE>
//        void operator()(Event const&,FSM& fsm,STATE& )
//        {
//            fsm.settingsData.window->Show(false);
//        }
//    };

//    struct settingsTag
//    {
//        //settingsState settings;
//    };
//    typedef msm::front::euml::func_state<settingsTag,settingsEntry,settingsExit> settings;

//    //transitions

//    //initial state
//    typedef gameOn initial_state;

//    struct transition_table : mpl::vector<
//    msm::front::Row < gameOn  ,  settingsOpen        , settings , msm::front::none  , msm::front::none                 >,
//    msm::front::Row < settings,  settingsClose       , gameOn   , msm::front::none  , msm::front::none                 >
//    > {};

//};

//typedef msm::back::state_machine<msmManager> stateMachine;
//} //namespace


//#endif // MSMMANAGER_H
