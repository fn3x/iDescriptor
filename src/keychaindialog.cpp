/*
 * iDescriptor: A free and open-source idevice management tool.
 *
 * Copyright (C) 2025 Uncore <https://github.com/uncor3>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "keychaindialog.h"
#include "settingsmanager.h"
#include <QApplication>
#include <QCheckBox>
#include <QLabel>
#include <QMediaPlayer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVideoWidget>

KeychainDialog::KeychainDialog(QWidget *parent)
    : QDialog(parent), m_player(nullptr), m_videoWidget(nullptr),
      m_mainLayout(nullptr), m_okButton(nullptr), m_titleLabel(nullptr),
      m_descriptionLabel(nullptr), m_dontShowAgainCheckbox(nullptr)
{
    setupUI();
    setupVideo();
}

KeychainDialog::~KeychainDialog()
{
    if (m_player) {
        m_player->stop();
    }
}

void KeychainDialog::setupUI()
{
    setWindowTitle("Keychain Access Required");
    setModal(true);
    setMinimumSize(600, 450);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    m_mainLayout->setSpacing(15);

    // Title label
    m_titleLabel = new QLabel("Keychain Access Required");
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "QLabel#titleLabel { "
        "  font-size: 18px; font-weight: bold; margin-bottom: 10px;"
        "}");
    m_mainLayout->addWidget(m_titleLabel);

    // Description label
    m_descriptionLabel = new QLabel(
        "In order to sign in to App Store we use the keychain backend to "
        "safely store and retrieve your credentials. Please click on \"Always "
        "Allow\" when prompted. "
        "This is a security feature to protect your Apple ID credentials. You "
        "can disable this in Settings.");
    m_descriptionLabel->setAlignment(Qt::AlignCenter);
    m_descriptionLabel->setWordWrap(true);
    m_mainLayout->addWidget(m_descriptionLabel);

    // Video widget
    m_videoWidget = new QVideoWidget();
    m_videoWidget->setObjectName("videoWidget");
    m_videoWidget->setSizePolicy(QSizePolicy::Expanding,
                                 QSizePolicy::Expanding);
    m_videoWidget->setAspectRatioMode(
        Qt::AspectRatioMode::KeepAspectRatioByExpanding);
    m_videoWidget->setStyleSheet(
        "QVideoWidget#videoWidget { background-color: transparent; }");
    m_videoWidget->setMinimumHeight(250);
    m_mainLayout->addWidget(m_videoWidget, 1);

    m_dontShowAgainCheckbox = new QCheckBox("Don't show this again");
    m_mainLayout->addWidget(m_dontShowAgainCheckbox, 0, Qt::AlignCenter);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    m_skipSigningInButton = new QPushButton("Skip For Now");
    m_skipSigningInButton->setFixedHeight(40);

    m_okButton = new QPushButton("OK, I understand");
    m_okButton->setDefault(true);
    m_okButton->setFixedHeight(40);

    buttonsLayout->addWidget(m_skipSigningInButton);
    buttonsLayout->addWidget(m_okButton);

    m_mainLayout->addLayout(buttonsLayout, Qt::AlignCenter);

    connect(m_okButton, &QPushButton::clicked, this,
            &KeychainDialog::onOkClicked);
    connect(m_skipSigningInButton, &QPushButton::clicked, this,
            &KeychainDialog::onSkipSigningInClicked);
}

void KeychainDialog::setupVideo()
{
    m_player = new QMediaPlayer(this);
    m_player->setVideoOutput(m_videoWidget);
    m_player->setSource(QUrl("qrc:/resources/keychain.mp4"));

    // Loop the video
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::EndOfMedia) {
                    m_player->setPosition(0);
                    m_player->play();
                }
            });

    // Auto-play when ready
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus status) {
                if (status == QMediaPlayer::LoadedMedia) {
                    m_player->play();
                }
            });
}

void KeychainDialog::onOkClicked()
{
    if (m_dontShowAgainCheckbox && m_dontShowAgainCheckbox->isChecked()) {
        SettingsManager::sharedInstance()->setShowKeychainDialog(false);
    }

    if (m_player) {
        m_player->stop();
    }
    accept();
}

void KeychainDialog::onSkipSigningInClicked()
{
    if (m_dontShowAgainCheckbox && m_dontShowAgainCheckbox->isChecked()) {
        SettingsManager::sharedInstance()->setShowKeychainDialog(false);
    }

    if (m_player) {
        m_player->stop();
    }
    reject();
}