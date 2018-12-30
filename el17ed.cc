#include "globals.h"
#include <QLabel>
#include <QtWidgets>
#include <QBoxLayout>
#include <QScrollArea>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace bs = boost::filesystem;

namespace{

class SelectRepo : public QWidget{
public:
	SelectRepo() : QWidget(){
		setUp();
		findRepos();
	}

	void deleteItems()
	{
		QLayoutItem *item;
		while ((item = _main_page->takeAt(0)) != 0)
		{
			delete item->widget();
			delete item;
		}
	}

	void setUp()
	{
		_head_layout->addWidget(new QLabel("Select a repository:"));
		_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		_scroller->setWidget(_scroll_buttons);
		_scroller->setWidgetResizable(true);
		_scroller->show();
		_head_layout->addWidget(_scroller);
		_main_page->addStretch(1);
		_head_layout->setContentsMargins(150, 5, 150, 5);

		setLayout(_head_layout);
	}

	void findRepos()
	{
		std::vector<std::string> git_folders = findGit("..");

		for (int i = 0; i < (int)git_folders.size(); i++)
		{
			QPushButton * repo_button = new QPushButton(QString::fromStdString(git_folders[i]));
			// repo_button->setFixedWidth(400);
			_main_page->insertWidget(0, repo_button);
			QObject::connect(repo_button, &QPushButton::clicked, this, [this]{setPath();});
		}
	}

	void unselectRepo()
	{
		main_globals.SetRepo(NULL);
		main_globals.IncrementRepoChanged();
		for (int i = 0; i < _main_page->count(); ++i)
		{
  		QWidget *widget = _main_page->itemAt(i)->widget();
  		if (widget != NULL)
				widget->setStyleSheet("default");
		}
		main_globals.GetWindow()->SetStatusBar("⚠ No Repository Set!");
	}

	void noRepoWarning() {
		if( auto input = QMessageBox::question( this, tr("WARNING"),
			tr("No Repository found!\nDo you want to create a new one?"),
			QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel))
		{
			if( input == QMessageBox::Yes) {
				createRepo();
			}
		}
	}

	void createRepo()
	{
		std::string temp = ".";
		GITPP::REPO* r = new GITPP::REPO(GITPP::REPO::_create, temp.c_str());
		main_globals.SetRepo(r);
		r->commits().create("Initial commit\n");
		_repo_created = true;

		findRepos();
	}

	bool hasGit(const std::string &dir_path)
	{
		bs::path p (dir_path);
		bs::directory_iterator end_itr;

		for (bs::directory_iterator i(p); i != end_itr; ++i)
		{
			std::string current = i->path().string();
			std::string sub = current.substr (current.size()-4, current.size());

			if (sub == ".git")
				return true;
		}

		return false;
	}

	std::vector<std::string> findGit(const std::string &dir_path)
	{
		std::vector<std::string> gits;

		bs::path p (dir_path);
		bs::recursive_directory_iterator end_itr;

		for (bs::recursive_directory_iterator i(p); i != end_itr; ++i)
		{
			std::string current = i->path().string();
			if (is_directory(i->path()) && hasGit(current))
				gits.push_back(current);
		}

		if (gits.size() == 0) {
			noRepoWarning();
		}

		return gits;
	}

	void setPath()
	{
		QPushButton * qsender = qobject_cast<QPushButton*>(sender());
		for (int i = 0; i < _main_page->count(); ++i)
		{
			QWidget *widget = _main_page->itemAt(i)->widget();
			if (widget != NULL)
				widget->setStyleSheet("default");
		}

		if(qsender->text().toStdString() == _path)
		{
			unselectRepo();
			_path = "";
		}
		else
		{
			qsender->setStyleSheet("background-color: green");
			std::string temp(qsender->text().toStdString());
			_path = temp;

			setRepo();
		}
	}

	void setRepo()
	{
		main_globals.IncrementRepoChanged();
		GITPP::REPO *r = new GITPP::REPO(_path);
		main_globals.SetRepo(r);
		main_globals.GetWindow()->SetStatusBar("Repository Set!");
	}

private:
	std::string _path = "";
	QWidget* _scroll_buttons = new QWidget();
	QScrollArea* _scroller = new QScrollArea;
	QVBoxLayout* _head_layout = new QVBoxLayout;
	QVBoxLayout* _main_page = new QVBoxLayout(_scroll_buttons);
	bool _repo_created = false;
	};

INSTALL_TAB(SelectRepo, "1. Repositories");

}
