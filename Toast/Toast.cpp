﻿#include "Toast.h"

#include "ToastImpl.h"

Toast::Toast(QWidget* parent)
    : QWidget(parent)
{
}

Toast::~Toast()
{
}

void Toast::ShowTip(const QString& text, QWidget* parent)
{
    ToastImpl::ShowTip(text, parent);
}
