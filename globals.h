#ifndef GITP
#define GITP

#include "dispatch.h"
#include "gitpp.h"
#include "window.h"

#endif
/**
 * A class for safely holding and accessing the main
 * program and the current repo
 *
**/
class MainInterface {
public:
    MainInterface();
    void SetRepo(GITPP::REPO* r);
    bool RepoExist();
    GITPP::REPO* GetRepo();
    MainWindow* GetWindow();
    void SetWindow(MainWindow* w);
    int GetRepoChanged();
    void IncrementRepoChanged();
private:
    GITPP::REPO* _repo;
    MainWindow* _window;
    bool _is_repo;
    int _repo_changed;
};

extern MainInterface main_globals;
extern WidgetDispatcher myTabs;
