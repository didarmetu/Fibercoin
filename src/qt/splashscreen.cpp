// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "splashscreen.h"

#include "clientversion.h"
#include "init.h"
#include "networkstyle.h"
#include "ui_interface.h"
#include "util.h"
#include "version.h"

#ifdef ENABLE_WALLET
#include "wallet.h"
#endif

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QPainter>
#include <boost/bind.hpp>

SplashScreen::SplashScreen(Qt::WindowFlags f, const NetworkStyle* networkStyle) : QWidget(0, f), curAlignment(0)
{
    titleText = tr("Fibercoin");
    versionText =
        QString(tr("Version %1"))
            .arg(QString::fromStdString(FormatFullVersion()));
    copyrightTextBtc =
        QChar(0xA9) + QString(" 2009-%1 ").arg(COPYRIGHT_YEAR) +
        QString(tr("The Bitcoin Core developers"));
    copyrightTextDash =
        QChar(0xA9) + QString(" 2014-%1 ").arg(COPYRIGHT_YEAR) +
        QString(tr("The Dash Core developers"));
    copyrightTextPIVX =
        QChar(0xA9) + QString(" 2015-%1 ").arg(COPYRIGHT_YEAR) +
        QString(tr("The PIVX Core developers"));
    copyrightTextFBC =
        QChar(0xA9) + QString(" 2019-%1 ").arg(COPYRIGHT_YEAR) +
        QString(tr("The Fibercoin developers"));
    titleAddText = networkStyle->getTitleAddText();

    pixmap = networkStyle->getSplashImage();

    // Set window title
    setWindowTitle(titleText + " " + titleAddText);

    // Resize window and move to center of desktop, disallow resizing
    QRect r(QPoint(), pixmap.size());
    resize(r.size());
    setFixedSize(r.size());
    move(QApplication::desktop()->screenGeometry().center() - r.center());

    subscribeToCoreSignals();
}

SplashScreen::~SplashScreen()
{
    unsubscribeFromCoreSignals();
}

void SplashScreen::slotFinish(QWidget* mainWin)
{
    Q_UNUSED(mainWin);
    hide();
}

static void InitMessage(SplashScreen* splash, const std::string& message)
{
    QMetaObject::invokeMethod(splash, "showMessage",
        Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(message)),
        Q_ARG(int, Qt::AlignBottom | Qt::AlignHCenter),
        Q_ARG(QColor, QColor(255, 255, 255)));
}

static void ShowProgress(SplashScreen* splash, const std::string& title, int nProgress)
{
    InitMessage(splash, title + strprintf("%d", nProgress) + "%");
}

#ifdef ENABLE_WALLET
static void ConnectWallet(SplashScreen* splash, CWallet* wallet)
{
    wallet->ShowProgress.connect(boost::bind(ShowProgress, splash, _1, _2));
}
#endif

void SplashScreen::subscribeToCoreSignals()
{
    // Connect signals to client
    uiInterface.InitMessage.connect(boost::bind(InitMessage, this, _1));
    uiInterface.ShowProgress.connect(boost::bind(ShowProgress, this, _1, _2));
#ifdef ENABLE_WALLET
    uiInterface.LoadWallet.connect(boost::bind(ConnectWallet, this, _1));
#endif
}

void SplashScreen::unsubscribeFromCoreSignals()
{
    // Disconnect signals from client
    uiInterface.InitMessage.disconnect(boost::bind(InitMessage, this, _1));
    uiInterface.ShowProgress.disconnect(boost::bind(ShowProgress, this, _1, _2));
#ifdef ENABLE_WALLET
    if (pwalletMain)
        pwalletMain->ShowProgress.disconnect(boost::bind(ShowProgress, this, _1, _2));
#endif
}

void SplashScreen::showMessage(const QString& message, int alignment, const QColor& color)
{
    curMessage = message;
    curAlignment = alignment;
    curColor = color;
    update();
}

void SplashScreen::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    const int paddingLeft = 14;
    const int paddingTop = 400;
    const int titleVersionVSpace = 17;
    const int titleCopyrightVSpace = 32;

    float fontFactor = 1.0;
    const QString font = QApplication::font().family();

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter.drawPixmap(rect(), pixmap);
    painter.setPen(QColor(255, 255, 255));

    painter.setFont(QFont(font, 28 * fontFactor));
    QFontMetrics fm = painter.fontMetrics();
    if (fm.width(titleText) > 160)
        fontFactor = 0.75;

    painter.setFont(QFont(font, 28 * fontFactor));
    painter.drawText(paddingLeft, paddingTop, titleText);

    painter.setFont(QFont(font, 15 * fontFactor));
    painter.drawText(
        paddingLeft,
        paddingTop + titleVersionVSpace,
        versionText);

    painter.setFont(QFont(font, 10 * fontFactor));
    painter.drawText(
        paddingLeft,
        paddingTop + titleCopyrightVSpace,
        copyrightTextBtc);
    painter.drawText(
        paddingLeft,
        paddingTop + titleCopyrightVSpace + 12,
        copyrightTextDash);
    painter.drawText(
        paddingLeft,
        paddingTop + titleCopyrightVSpace + 24,
        copyrightTextPIVX);
    painter.drawText(
        paddingLeft,
        paddingTop + titleCopyrightVSpace + 36,
        copyrightTextFBC);

    if (!titleAddText.isEmpty()) {
        QFont boldFont(font, 10 * fontFactor);
        boldFont.setWeight(QFont::Bold);
        painter.setFont(boldFont);
        fm = painter.fontMetrics();

        const int titleAddTextWidth = fm.width(titleAddText);
        painter.drawText(
            width() - titleAddTextWidth - 10,
            height() - 25,
            titleAddText);
    }

    const QRect messageRect = rect().adjusted(5, 5, -5, -5);
    painter.setPen(curColor);
    painter.drawText(messageRect, curAlignment, curMessage);
}

void SplashScreen::closeEvent(QCloseEvent* event)
{
    StartShutdown(); // allows an "emergency" shutdown during startup
    event->ignore();
}
