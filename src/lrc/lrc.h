
#ifndef __LRC_H__
#define __LRC_H__

#include "lrc_export.h"

LRCX_BEGIN_NS

class LRC_EXPORT LRC
{
public:

	struct Line
	{
		int time; // in milliseconds
		std::string lyrics;
		int index; // index in vector

		Line()
			: time(0)
			, index(-1)
		{}

		Line(int t, const std::string &lrc)
			: time(t)
			, lyrics(lrc)
			, index(-1)
		{}
	};

public:
	LRC();
	LRC(const LRC &other);
	~LRC();

	void addTag(const std::string &tag, const std::string &value);

	void setOffset(int offset);
	int getOffset() const;

	void append(const Line &line);
	void clear();

	int count() const;
	int attrCount() const;

	const Line* getLine(int index) const;
	const Line* getLineByTime(int time) const;
	const std::vector<Line>& getLines() const;

	std::string attribute(const std::string &name) const;
	const std::map<std::string, std::string>& attributes() const;

	// sort lyrics by time
	void sort();

	LRC& operator=(const LRC &other);

protected:
	void assign(const LRC &other);

private:
	std::map<std::string, std::string> m_tags;
	std::vector<Line> m_lyrics;
};

LRCX_END_NS

#endif // __LRC_H__
