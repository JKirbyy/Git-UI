#include "globals.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QApplication>
#include <QScrollArea>
#include <QtDebug>


namespace{

class numCommit : public QWidget{
public:
	numCommit() : QWidget(){

		setUp();
		setLayout(_empty_layout);

	}
	void showEvent( QShowEvent* event ) {
	QWidget::showEvent( event ); //When tab is clicked repopulate commit buttons
	int no = 0;
	if(main_globals.RepoExist()) {
		for (auto i : main_globals.GetRepo()->commits())
		{
			(void)i;
			no ++;
		}
	}
	_number_of_commits->setText(QString::number(no));
}

void getUsers() {
	int no = 0;

	if(main_globals.RepoExist()) {
		for (auto i : main_globals.GetRepo()->commits())
		{ _string_vector.push_back(i.author());
			for (int p = 0; p <  int(_string_vector.size()); p++){
				if(_string_vector[p] == i.author()){
					no++;
				}
			}
		}
	}
	_number_of_users->setText(QString::number(no));

}

private:
	QHBoxLayout* _empty_layout = new QHBoxLayout;
	std::vector<std::string> _string_vector;
	QLineEdit* _textField = new QLineEdit();
	QVBoxLayout* _vert_layout = new QVBoxLayout;
	QLabel* _number_of_commits = new QLabel();
	QLabel* _number_of_users = new QLabel();
	// QPushButton* _perform_search = new QPushButton("", this);

	void setUp() {

		_empty_layout->addWidget(_number_of_commits);
		_empty_layout->addWidget(new QLabel("Not implemented"));
		_empty_layout->addWidget(new QLabel("Not implemented"));
		_empty_layout->addWidget(new QLabel("Not implemented"));

	}

};
}



INSTALL_TAB(numCommit, "5. Statistics");
