#include "encodingchooser.h"
#include "ui_encodingchooser.h"

#include <QTextCodec>
#include <QTextStream>

LRCX_BEGIN_NS

EncodingChooser::EncodingChooser(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::EncodingChooser)
{
	m_ui->setupUi(this);

	initEncodings();

	m_ui->tePreview->setReadOnly(true);

	connect(m_ui->cbEncoding, SIGNAL(activated(int)), this, SLOT(updatePreview()));
}

void EncodingChooser::initEncodings()
{
	foreach (int mib, QTextCodec::availableMibs())
	{
		QTextCodec *codec = QTextCodec::codecForMib(mib);
		m_ui->cbEncoding->addItem(codec->name(), mib);
	}
}

void EncodingChooser::updatePreview()
{
	int mib = m_ui->cbEncoding->currentData().toInt();
	QTextCodec *codec = QTextCodec::codecForMib(mib);

	QTextStream in(&m_data);
	in.setAutoDetectUnicode(false);
	in.setCodec(codec);

	m_ui->tePreview->setPlainText(in.readAll());
}

void EncodingChooser::setCurrentEncoding(const QString &encoding)
{
	QTextCodec *codec = QTextCodec::codecForName(encoding.toLocal8Bit());
	if (codec)
	{
		int mib = codec->mibEnum();
		int index = m_ui->cbEncoding->findData(mib);
		if (index != -1)
		{
			m_ui->cbEncoding->setCurrentIndex(index);
			updatePreview();
		}
	}
}

QString EncodingChooser::getCurrentEncoding() const
{
	int mib = m_ui->cbEncoding->currentData().toInt();
	return QTextCodec::codecForMib(mib)->name();
}

void EncodingChooser::setPreviewData(const QByteArray &data)
{
	m_data = data;
	updatePreview();
}

LRCX_END_NS
