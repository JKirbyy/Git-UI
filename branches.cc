#include "globals.h"

#include <QLabel> //check if any of these includes needed
#include <QHBoxLayout>
#include <QPushButton>

namespace{

class BranchTab : public QWidget{
public:
	BranchTab() : QWidget(){

		GITPP::REPO r; //need to memebr variale?
		//left_layout->setContentsMargins(5,5,20,5);
		left_layout->addWidget(new QLabel("Branches:"));

		for(GITPP::BRANCH i : r.branches())
		{
			QPushButton* branchButton = new QPushButton(QString::fromStdString(i.name())); //check this and check banchbutton naming convetion
			connect(branchButton, &QPushButton::clicked, this, [i,this]{branchCheckout(i);}); //ask team why [] stuff?
			left_layout->addWidget(branchButton);
		}
		right_layout->addWidget(branch_name);
		left_layout->addStretch(1);
		main_layout->addLayout(left_layout);
		main_layout->addLayout(right_layout);
		setLayout(main_layout);
	}
		void branchCheckout(GITPP::BRANCH branch)
		{
			branch_name->setText(QString::fromStdString(branch.name()));
		}

private:
	QVBoxLayout* left_layout=new QVBoxLayout();
	QVBoxLayout* right_layout=new QVBoxLayout();
	QHBoxLayout* main_layout=new QHBoxLayout();
	QLabel* branch_name = new QLabel();

};


INSTALL_TAB(BranchTab, "Branches Tab");

}
