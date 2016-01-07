/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __ENCODING_CHOOSER_H__
#define __ENCODING_CHOOSER_H__

#include "config.h"

#include <QDialog>

namespace Ui {
class EncodingChooser;
}

LRCX_BEGIN_NS

class EncodingChooser : public QDialog
{
	Q_OBJECT
public:
	EncodingChooser(QWidget *parent = 0);

	void setCurrentEncoding(const QString &encoding);
	QString getCurrentEncoding() const;

	void setPreviewData(const QByteArray &data);

private:
	void initEncodings();

private Q_SLOTS:
	void updatePreview();

private:
	std::shared_ptr<Ui::EncodingChooser> m_ui;
	QByteArray m_data;
};

LRCX_END_NS

#endif // __ENCODING_CHOOSER_H__
