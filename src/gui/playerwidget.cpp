#include "playerwidget.h"
#include "ui_playerwidget.h"

LRCX_BEGIN_NS

PlayerWidget::PlayerWidget(QWidget *parent)
	: QDockWidget(parent)
	, m_ui(new Ui::PlayerWidget)
{
	m_ui->setupUi(this);

	connect(m_ui->btn_Open, SIGNAL(clicked(bool)), this, SLOT(onBtnOpen_Clicked()));
	connect(m_ui->btn_PlayPause, SIGNAL(clicked(bool)), this, SLOT(onBtnPlayPause_Clicked()));
	connect(m_ui->slider_Duration, SIGNAL(valueChanged(int)), this, SLOT(onSliderDuration_Changed(int)));
}

PlayerWidget::~PlayerWidget()
{

}

void PlayerWidget::onBtnOpen_Clicked()
{

}

void PlayerWidget::onBtnPlayPause_Clicked()
{

}

void PlayerWidget::onSliderDuration_Changed(int value)
{

}

LRCX_END_NS
