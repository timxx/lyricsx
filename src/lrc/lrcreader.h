#ifndef __LRC_READER_H__
#define __LRC_READER_H__

#include "lrc.h"

LRCX_BEGIN_NS

class LRC_EXPORT LrcReader
{
public:
	LrcReader();
	explicit LrcReader(const std::string &file);
	explicit LrcReader(const char *buffer, int size = -1);
	~LrcReader();

	void setFile(const std::string &file);
	void setBuffer(const char *buffer, int size = -1);
	bool parse();

	const LRC& getLrc() const;
	const std::string& getFile() const;

private:
	bool parseLine(const std::string &line);
	int parseTime(const std::string &tag);

private:
	std::shared_ptr<LRC> m_lrc;
	std::string m_file;
	std::string m_buffer;
};

LRCX_END_NS

#endif // __LRC_READER_H__
