//
// Created by Ryuki
//

// You may need to build the project (run Qt uic code generator) to get "ui_MemoListItem.h" resolved

#include "MemoListItem.h"
#include "ui_MemoListItem.h"

namespace mosme
{
    MemoListItem::MemoListItem(QWidget* parent) :
            QWidget(parent), ui(new Ui::MemoListItem), model(new Memo)
    {
        ui->setupUi(this);
    }

    MemoListItem::~MemoListItem()
    {
        delete ui;
    }

    void MemoListItem::SetModel(const Memo &memo)
    {
        *this->model = memo;
    }

    Memo* MemoListItem::GetModel() const
    {
        return model;
    }
} // mosme
