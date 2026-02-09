#pragma once

#include "toast_global.h"

#include <QMessageBox>
#include <QReadWriteLock>
#include <QWidget>

#ifndef TOAST_TIP
#define TOAST_TIP(text) Toast::ShowTip(text)
#endif // !TOAST_TIP

#ifndef TOAST_TIP_P
#define TOAST_TIP_P(text, type, parent) Toast::ShowTip(text, type, parent)
#endif // !TOAST_TIP_P

#ifndef TOAST_TIP_T
#define TOAST_TIP_T(text, type) Toast::ShowTip(text, type)
#endif // !TOAST_TIP_T

#ifndef TOAST_INFO
#define TOAST_INFO(text) Toast::ShowTip(text, QMessageBox::Information)
#endif // !TOAST_INFO

#ifndef TOAST_WARNING
#define TOAST_WARNING(text) Toast::ShowTip(text, QMessageBox::Warning)
#endif // !TOAST_WARNING

#ifndef TOAST_ERROR
#define TOAST_ERROR(text) Toast::ShowTip(text, QMessageBox::Critical)
#endif // !TOAST_ERROR

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
    static void ShowTip(const QString& text, QMessageBox::Icon type = QMessageBox::Information, QWidget* parent = nullptr);
};
