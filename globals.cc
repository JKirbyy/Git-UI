#include "globals.h"

MainInterface::MainInterface() {
    //defualting the pointers
    _repo = NULL;
    _window = NULL;
    _is_repo = false;//this is used to check the repo
    _repo_changed = 1;
}
void MainInterface::SetRepo(GITPP::REPO* r) {
    //set the currently used repo
    if (r == NULL) { _is_repo = false; }
    else { _is_repo = true; }
    _repo = r;
}

int MainInterface::GetRepoChanged() {
  return _repo_changed;

}
void MainInterface::IncrementRepoChanged() {
  _repo_changed++;
}

//the bool is used to see if we have a repo in use
bool MainInterface::RepoExist() { return _is_repo; }
//get the current repo if in use
GITPP::REPO* MainInterface::GetRepo() { return _repo; }
//get the whole window application
MainWindow* MainInterface::GetWindow() { return _window; }
//set the whole application window
void MainInterface::SetWindow(MainWindow* w) { _window = w; }

MainInterface main_globals;
WidgetDispatcher myTabs;//extends the tabs and maini_globals throughout
