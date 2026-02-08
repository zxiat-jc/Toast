#include "ToastImpl.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>

#include "ui_Toast.h"

ToastImpl::ToastImpl(QWidget* parent)
    : Toast(parent)
    , ui(new Ui::ToastClass())
{
    ui->setupUi(this);
    // 无边框 无任务栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Tool);
    // 背景透明
    setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setType(QMessageBox::Information);

}

ToastImpl::~ToastImpl()
{
}

void ToastImpl::setType(QMessageBox::Icon type)
{
    QString style;
    switch (type) {
    case QMessageBox::Information:
        style = QStringLiteral("QLabel { color: #FFFFFF; background-color: #2196F3; padding: 6px 12px; border-radius: 4px; }");
        break;
    case QMessageBox::Warning:
        style = QStringLiteral("QLabel { color: #FFFFFF; background-color: #FF9800; padding: 6px 12px; border-radius: 4px; }");
        break;
    case QMessageBox::Critical:
        style = QStringLiteral("QLabel { color: #FFFFFF; background-color: #F44336; padding: 6px 12px; border-radius: 4px; }");
        break;
    default:
        style = QStringLiteral("QLabel { color: #FFFFFF; background-color: #2196F3; padding: 6px 12px; border-radius: 4px; }");
        break;
    }
    ui->label->setStyleSheet(style);
}

void ToastImpl::setText(const QString& text)
{
    ui->label->setText(text);
}

void ToastImpl::showAnimation(int timeout /*= 2000*/)
{
    // 开始动画
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    this->show();

    QTimer::singleShot(timeout, [&] {
        // 结束动画
        QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [this] {
            {
                QWriteLocker locker(&Lock());
                // 删除this
                Widgets().removeOne(this);
            }
            this->close();
            // 关闭后析构
            this->deleteLater();
        });
    });
}

void ToastImpl::ShowTip(const QString& text, QMessageBox::Icon type,QWidget* parent)
{
    QTimer::singleShot(0, QCoreApplication::instance(), [text, type,parent]() {
        {
            QReadLocker locker(&Lock());
            // 存在的全部升高宽度+20px
            for (auto&& wid : Widgets()) {
                wid->move(wid->x(), wid->y() - wid->height() - 10);
            }
        }
        ToastImpl* toast = new ToastImpl();
        {
            QWriteLocker locker(&Lock());
            Widgets().append(toast);
        }
        // 置顶
        toast->setWindowFlags(toast->windowFlags() | Qt::WindowStaysOnTopHint);
        toast->setType(type);
        toast->setText(text);
        // 设置完文本后调整下大小
        toast->adjustSize();

        if (parent == nullptr) {
            // 默认显示位于主屏的70%高度位置，依次叠加升高宽度+20px
            QScreen* pScreen = QGuiApplication::primaryScreen();
            toast->move((pScreen->size().width() - toast->width()) / 2, pScreen->size().height() * 7 / 10);
        } else {
            // 显示在parent的正中间
            toast->move(parent->x() + (parent->width() - toast->width()) / 2, parent->y() + (parent->height() - toast->height()) / 2);
        }
        toast->showAnimation();
        qInfo() << text;
    });
}

void ToastImpl::paintEvent(QPaintEvent* event)
{
    QPainter paint(this);
    auto kBackgroundColor = QColor(255, 255, 255);
    // 透明度
    kBackgroundColor.setAlpha(0.0 * 255);
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.setPen(Qt::NoPen);
    // 设置画刷形式
    paint.setBrush(QBrush(kBackgroundColor, Qt::SolidPattern));
    paint.drawRect(0, 0, width(), height());
}

QList<QWidget*>& ToastImpl::Widgets()
{
    static QList<QWidget*> widgets {};
    return widgets;
}

QReadWriteLock& ToastImpl::Lock()
{
    static QReadWriteLock lock;
    return lock;
}