#include "ToastImpl.h"

#include <QCoreApplication>
#include <QDebug>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QTimer>
#include <QStyle>
#include <QBuffer>

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
}

ToastImpl::~ToastImpl()
{
}


void ToastImpl::setText(const QString& text)
{
    ui->label->setTextFormat(Qt::RichText);
    ui->label->setWordWrap(false);
    ui->label->setAlignment(Qt::AlignCenter);
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
        auto calcTargetX = [parent](QWidget* widget) -> int {
            if (parent == nullptr) {
                QScreen* pScreen = QGuiApplication::primaryScreen();
                return (pScreen->size().width() - widget->width()) / 2;
            }
            const QPoint parentTopLeft = parent->mapToGlobal(QPoint(0, 0));
            return parentTopLeft.x() + (parent->width() - widget->width()) / 2;
        };

        {
            QReadLocker locker(&Lock());
            // 存在的全部升高宽度+20px
            for (auto&& wid : Widgets()) {
                wid->move(calcTargetX(wid), wid->y() - wid->height() - 10);
            }
        }
        ToastImpl* toast = new ToastImpl();
        {
            QWriteLocker locker(&Lock());
            Widgets().append(toast);
        }
        // 置顶
        toast->setWindowFlags(toast->windowFlags() | Qt::WindowStaysOnTopHint);

         auto iconHtml = [type]() -> QString {
            QStyle::StandardPixmap sp = QStyle::SP_MessageBoxInformation;
            switch (type) {
            case QMessageBox::Warning:
                sp = QStyle::SP_MessageBoxWarning;
                break;
            case QMessageBox::Critical:
                sp = QStyle::SP_MessageBoxCritical;
                break;
            default:
                sp = QStyle::SP_MessageBoxInformation;
                break;
            }
            QPixmap pixmap = qApp->style()->standardIcon(sp).pixmap(16, 16);
            QByteArray bytes;
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "PNG");
            return QString("<img src=\"data:image/png;base64,%1\"/>").arg(QString::fromLatin1(bytes.toBase64()));
        };


        QString prefix;//前缀
        switch (type) {
        case QMessageBox::Information:
            prefix = "消息:";
            break;
        case QMessageBox::Warning:
            prefix = "警告:";
            break;
        case QMessageBox::Critical:
            prefix = "严重错误:";
            break;
        default:
            break;
        }

       toast->setText(QString(
            "<table cellspacing=\"0\" cellpadding=\"0\">"
            "<tr>"
            "<td style=\"vertical-align:middle; padding-right:6px;\">%1</td>"
            "<td style=\"vertical-align:middle; color:#FFFFFF;\">%2%3</td>"
            "</tr>"
            "</table>")
                .arg(iconHtml())
                .arg(prefix)
                .arg(text));
        toast->adjustSize();

        if (parent == nullptr) {
            // 默认显示位于主屏的70%高度位置，依次叠加升高宽度+20px
            QScreen* pScreen = QGuiApplication::primaryScreen();
            toast->move(calcTargetX(toast), pScreen->size().height() * 7 / 10);
        } else {
            // 显示在parent的正中间
            const QPoint parentTopLeft = parent->mapToGlobal(QPoint(0, 0));
            toast->move(calcTargetX(toast), parentTopLeft.y() + (parent->height() - toast->height()) / 2);
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