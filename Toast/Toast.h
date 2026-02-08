#pragma once

#include "toast_global.h"

#include <QReadWriteLock>
#include <QMessageBox>
#include <QWidget>

#ifndef TOAST_TIP
#define TOAST_TIP(text) Toast::ShowTip(text)
#endif // !TOAST_TIP

#ifndef TOAST_TIP_P
#define TOAST_TIP_P(text, parent) Toast::ShowTip(text, parent)
#endif // !TOAST_TIP_P

class TOAST_EXPORT Toast : public QWidget {
    Q_OBJECT

public:
    Toast(QWidget* parent = nullptr);
    ~Toast();

    virtual void setText(const QString& text) = 0;

    virtual void showAnimation(int timeout = 2000) = 0; // 动画方式show出，默认2秒后消失

public:
    /**
     * @brief 静态调用
     * @param text 文字内容
     */
    static void ShowTip(const QString& text, QMessageBox::Icon type= QMessageBox::Information, QWidget* parent = nullptr);
};
