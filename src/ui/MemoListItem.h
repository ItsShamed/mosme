//
// Created by Ryuki
//

#pragma once

#include "IModelBackedWidget.h"
#include "../network/Memo.h"

namespace mosme
{
    QT_BEGIN_NAMESPACE
    namespace Ui { class MemoListItem; }
    QT_END_NAMESPACE

    class MemoListItem : public QWidget, public IModelBackedWidget<Memo>
    {
    Q_OBJECT

    public:
        explicit MemoListItem(QWidget* parent = nullptr);
       
        void SetModel(const Memo &) override;

        Memo* GetModel() const override;

        ~MemoListItem() override;

    private:
        Ui::MemoListItem* ui;
        Memo* model;
    };
} // mosme
