#include "globals.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QtDebug>

namespace {

class Commits : public QWidget {
//Q_OBJECT
public:
	Commits() : QWidget() {
		setUpLayout();
		_main_layout->addLayout(_left_layout);
		_main_layout->addLayout(_commit_details);
		setLayout(_main_layout);
	}

	void setUpLayout() {
		_perform_search->setIcon(QIcon("search_icon.png")); //Setup and connect search bar/button
		_perform_search->setFixedSize( 25, 25 );
		connect(_perform_search, &QPushButton::clicked, [this]{setSearch();});

		_textField->setPlaceholderText("Search commits");
		_textField->show();
		_search_field->addWidget(_textField);
		_search_field->addWidget(_perform_search, 1);

		_button_layout->setContentsMargins(5,5,5,5);
		_scroller->setMaximumWidth(360); //Set size and policies of scroller
		_scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		_scroller->setWidget(_scroll_buttons); //add layout containing buttons to scroller
		_scroller->setWidgetResizable(true);
		_scroller->show();
		_left_layout->addWidget(new QLabel("Your commits:"));
		_left_layout->addWidget(_scroller);


		_commit_details->addLayout(_search_field); //Add search bar and info labels to right half
		_commit_details->addWidget(_author);
		_commit_details->addWidget(_message);
		_commit_details->addWidget(_time);
		_commit_details->addWidget(_id);

		_author->setWordWrap(true);
		_message->setWordWrap(true);
		_time->setWordWrap(true);
		_id->setWordWrap(true);
	}

	void runSearch() {
		_commits.clear(); //Clear commits so ready to repopulate

		QLayoutItem *button; //Remove all commit buttons
		while ((button = _button_layout->takeAt(0)) != 0) {
		    delete button->widget();
		    delete button;
		}
		//reset all info labels
		_id->setText(QString::fromStdString(""));
		_time->setText(QString::fromStdString(""));
		_author->setText(QString::fromStdString(""));
		_message->setText(QString::fromStdString(""));


		bool no_results = true;
		//Add commits to vector
		if(main_globals.RepoExist()) {
			for(GITPP::COMMIT i : main_globals.GetRepo()->commits()) {
				_commits.push_back(i);
			}
			//Create buttons from commits within vector
			for(GITPP::COMMIT i : _commits) {
				if (i.message().find(_search) != std::string::npos || _search == "") {
					QPushButton* button = new QPushButton(reduceText(i.message()), this);
					button->setEnabled(true);
					button->setMaximumWidth(400);
					_button_layout->addWidget(button,1);
					connect(button, &QPushButton::clicked, this, [i,this]{setDetails(i);});
					no_results = false;
				}
			}
		}

		if (no_results) {
			_button_layout->addWidget(new QLabel("No results found"));
		}
		_button_layout->addStretch(1);
	}

	QString reduceText(std:: string text) {
		std::string button_text = text;
		if (button_text.size() > 45 ){ //If commits text is too long, shorted text and add ...
			button_text = button_text.substr(0, 45);
			button_text += "...";
		}
		else { //remove end newline character from commits button text
			button_text = button_text.substr(0, button_text.size() - 1);
		}

		return QString::fromStdString(button_text);
	}

	void setSearch() {
		_search = _textField->text().toStdString();
		runSearch();
	}


	void setDetails(GITPP::COMMIT commit) {
		_id->setText(QString::fromStdString("ID = " + commit.id()));
		_time->setText(QString::fromStdString("Time = " + commit.time()));
		_author->setText(QString::fromStdString("Author = " + commit.author()));
		_message->setText(QString::fromStdString("Message = " + commit.message()));

		//Set all buttons to default colour
		for (int i = 0; i < _button_layout->count(); ++i) {
			QWidget *widget = _button_layout->itemAt(i)->widget();
			if (widget != NULL) {
				widget->setStyleSheet("default");
			}
		} //set current selected button to green
		QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
		buttonSender->setStyleSheet("background-color: green");
	}

	void showEvent( QShowEvent* event ) {
    QWidget::showEvent( event ); //When tab is clicked repopulate commit buttons
		runSearch();
}

private:
	std::vector<QPushButton*> _commit_buttons;
	std::vector<GITPP::COMMIT> _commits;

	QWidget* _scroll_buttons = new QWidget();
	QScrollArea* _scroller = new QScrollArea;
	QHBoxLayout* _main_layout = new QHBoxLayout;
	QVBoxLayout* _button_layout = new QVBoxLayout(_scroll_buttons);
	QVBoxLayout* _left_layout = new QVBoxLayout();
	QVBoxLayout* _commit_details = new QVBoxLayout;
	QHBoxLayout* _search_field = new QHBoxLayout;
	std::string _search = "";
	QPushButton* _perform_search = new QPushButton("", this);
	QLineEdit* _textField = new QLineEdit();
	QLabel* _id = new QLabel();
	QLabel* _time = new QLabel();
	QLabel* _author = new QLabel();
	QLabel* _message = new QLabel();

};


INSTALL_TAB(Commits, "3. Search Commits");

}
