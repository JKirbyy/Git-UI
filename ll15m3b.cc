#include "globals.h"

#include <QLabel>
#include <QtWidgets>
#include <QLabel>
#include <QtWidgets>
#include <QBoxLayout>
#include <QScrollArea>

#include <iostream>
#include <string>
#include <vector>

namespace{
/**
 * The start of config class/tab
 * Used to edit the users git config files 
**/
class Config : public QWidget{
public:
	Config() : QWidget(){ SetUp(); }//constuctor set up.
	
	void showEvent( QShowEvent* event ) {//called when ever the tab is pressed
    	QWidget::showEvent( event );
		ResetSearchBox(); 
		if(!main_globals.RepoExist()) {
			ClearButtons();//get rid of buttons and nothing found
			_left_layout->addWidget(new QLabel("No results found"));
			_left_layout->addStretch(1);
		}
	}

	void SetUp() {//main setup for the cofic tab
		_layout = new QHBoxLayout();//setting private layouts
		_label_layout = new QVBoxLayout();
		_label_layout->addWidget(new QLabel("Click to edit your config settings:"));
		_left_layout = new QVBoxLayout(_scroll_buttons);
		_right_layout = new QVBoxLayout();
		_padding = new QVBoxLayout();
		//if there is a repo make buttons
		if(main_globals.RepoExist()) { //safety checking
			UpdateButtons();//Displays all the buttons
		}
		//setting up scroll bar for buttons
		_scroller->setWidget(_scroll_buttons);
		_scroller->setMaximumWidth(360);
		_scroller->setWidgetResizable(true);
		_scroller->show();

		_left_layout->addStretch(1);//adding stretches
		_label_layout->addWidget(_scroller,1);
		_layout->addLayout(_label_layout, 1);

		_name_field = new QLineEdit();//the line for user input
		_name_field->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
		_name_field->setMinimumWidth(200);//setting input size
		_name_field->hide();//will appear when needed

		_set = new QPushButton("Set", this);//the user set button
		_set->hide();//will appear when config selected
		//connecting the buttons to a moethod
		connect(_set, &QPushButton::clicked, this, &Config::OnClickedSet);
		//sorting right layout
		_right_layout->addWidget(new QLabel("Values:"));
		_right_layout->addWidget(_name_field,1);
		_right_layout->addWidget(_set);
		_right_layout->addStretch(1);
		//sorting padding for spacing
		_padding->addLayout(_right_layout, 1);
		_padding->addWidget(new QLabel(""));
		_padding->addStretch(1);
		_layout->addLayout(_padding,1);

		//setStyleSheet("background-color: LightGrey");//background
		setLayout(_layout);
	}

	//These two methods are called on clicking buttons
	void OnClickedPushButton() {
		for (int i = 0; i < _left_layout->count(); ++i) {
			QWidget *widget = _left_layout->itemAt(i)->widget();
			if (widget != NULL) {//called to reset colour of buttons
				widget->setStyleSheet("default");
			}
		}
		//grab the button that was clicked, retrieve the button you have clicked
		QPushButton* buttonSender = qobject_cast<QPushButton*>(sender()); 
		buttonSender->setStyleSheet("background-color: green");
		std::string name_string (1,(char)buttonSender->text().toStdString().at(0));
		_current_config = &_config_vector[((std::stoi(name_string))-1)];
		//show the user input
		_name_field->show();//setting the current user config setting
		_name_field->setPlaceholderText(QString::fromStdString(_current_config->value()));
		_set->show();
	}
	void OnClickedSet()	{
		WarningConfig();//WARNING MESSAGE!
	}

	//Called for when you wish to change a setting, as safety
	void WarningConfig() {
		if( auto input = QMessageBox::question( this, tr("WARNING"),
			tr("If you change a config setting it may break your repository! Continue?"),
			//setting choices for the user in the setting of settings
			QMessageBox::Yes |
			QMessageBox::Cancel, QMessageBox::Cancel))
		{//if staements to check users input
			if( input == QMessageBox::Yes) {
				auto c = main_globals.GetRepo()->config();
				char message[122];
				std::string string_message (_name_field->text().toStdString());
				c.create(_current_config->name());
				strcpy(message, string_message.c_str());
				c[_current_config->name()] = message;//set user data

				QMessageBox::information(this, tr("Set"), tr("Change made!"));
				ResetSearchBox();
			}else if (input == QMessageBox::Cancel) {
				ResetSearchBox();
			}else {
				ResetSearchBox();
			}
		}
	}

	void UpdateButtons() {
		ClearButtons();//clear and reset user config buttons
		int counter = 1;
		for (auto i : main_globals.GetRepo()->config()) {
			QPushButton* tmp = new QPushButton(QString::fromStdString(
				std::to_string(counter) + " " + i.name()),this);
			//Reconnect to the buttons
			connect(tmp, &QPushButton::clicked,this, &Config::OnClickedPushButton);
			tmp->setStyleSheet("default");
			_left_layout->addWidget(tmp);
			counter++;
			_config_vector.push_back(i);//adding them to a helpful vector
		}
		_left_layout->addStretch(1);
	}
	void ClearButtons()	{//deletes all buttons
		_config_vector.clear();
		QLayoutItem *button;
		while ((button = _left_layout->takeAt(0)) != 0) { 
			delete button->widget(); delete button;
		}
	}
	void ResetSearchBox() {//resets my search box nicely
		_name_field->setText("");
		_name_field->hide();
		_set->hide();
		if (main_globals.RepoExist()){
			//checks if there is a repo, then updates buttons
			UpdateButtons();
		}
	}

private:
	QScrollArea* _scroller = new QScrollArea;
	QWidget* _scroll_buttons = new QWidget();
	QLineEdit* _name_field;
	QPushButton* _set;
	std::vector<GITPP::CONFIG::ITEM> _config_vector;
	GITPP::CONFIG::ITEM* _current_config;
	QHBoxLayout* _layout;
	QVBoxLayout* _right_layout;
	QVBoxLayout* _left_layout;
	QVBoxLayout* _label_layout;
	QVBoxLayout* _padding;
};

INSTALL_TAB(Config, "4. Custom Config");
}
