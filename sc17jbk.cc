#include "globals.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <iostream>
#include <QScrollArea>
#include <QtWidgets>
using namespace std; //for debug


namespace{

class BranchTab : public QWidget{
public:
	BranchTab() : QWidget(){Layout();}
	void Layout() //sets up initial layout
	{
			_main_layout->addWidget(new QLabel("Select branch to checkout:"));
			_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			_scroll_area->setWidgetResizable(true);
			_scroll_area->show();
			_scroll_area_widget->setLayout(_scroll_layout);
			_scroll_area->setWidget(_scroll_area_widget);
			_main_layout->addWidget(_scroll_area);
			_main_layout->setContentsMargins(150, 5, 150, 5); //centres the scroll area
			setLayout(_main_layout);
	}

	void showEvent(QShowEvent* event) //called whenever tab is opened
	{
		QWidget::showEvent( event );
		if (_check != main_globals.GetRepoChanged()) //tabs should only update if repo is changed and this checks that
		{
			_check = main_globals.GetRepoChanged(); //sets check to current changed value so it can compare when tab is opened again
			deleteButtons();
			if (main_globals.RepoExist()) //if a current repo is selected
			{
				for(GITPP::BRANCH i: main_globals.GetRepo()->branches())
				{
					QPushButton* branch_button = new QPushButton(QString::fromStdString(i.name()));
					connect(branch_button, &QPushButton::clicked, this, [i,this]{branchCheckout(i);}); //connects button to branchCheckout function
					if (QString::fromStdString(i.name()) == "master") //shows master is default checked out
					{
						branch_button->setStyleSheet("background-color: green");
					}
					_scroll_layout->insertWidget(1, branch_button);
					_scroll_layout->addStretch(1);
				}
			}
		}
	}

	void deleteButtons()
	{
		QLayoutItem *button;
		while ((button = _scroll_layout->takeAt(0)) != 0) //pops each item in layout until empty
		{
			delete button->widget();
			delete button;
		}
	}

	void branchCheckout(GITPP::BRANCH branch) //called on button press
	{
		try
		{
			main_globals.GetRepo()->checkout(branch.name()); //checkout branch
			QPushButton * clicked_button = qobject_cast<QPushButton*>(sender()); //the button that called the function
			QList<QPushButton *> buttons = findChildren<QPushButton *>();

			for (int i = 0; i < buttons.count(); i++)  //refreshes all button colours
			{
				//cout << i;
				QPushButton* temp = buttons.value(i);
				temp->setStyleSheet("default");
			}

			clicked_button->setStyleSheet("background-color: green");
		}

		catch (const std::exception& ex) //catches errors trying to check out branch
		{
				QMessageBox::question( this, tr("ERROR"),
				tr(ex.what()),QMessageBox::Ok, QMessageBox::Ok); //passes exception message to pop up box
		}

	}

private:
	QVBoxLayout* _scroll_layout=new QVBoxLayout();
	QVBoxLayout* _main_layout=new QVBoxLayout();
	QWidget* _scroll_area_widget = new QWidget;
	QScrollArea* _scroll_area = new QScrollArea;
	int _check = 0;

};


INSTALL_TAB(BranchTab, "2. Branches");

}
