/*  ===========================================================================
*
*   This file is part of HISE.
*   Copyright 2016 Christoph Hart
*
*   HISE is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   HISE is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with HISE.  If not, see <http://www.gnu.org/licenses/>.
*
*   Commercial licenses for using HISE in an closed source project are
*   available on request. Please visit the project's website to get more
*   information about commercial licensing:
*
*   http://www.hise.audio/
*
*   HISE is based on the JUCE library,
*   which must be separately licensed for closed source applications:
*
*   http://www.juce.com
*
*   ===========================================================================
*/

#pragma once

namespace hise {
using namespace juce;

struct ViewportWithScrollCallback : public Viewport
{
	class Listener
	{
	public:

		virtual ~Listener() {}

		virtual void scrolled(Rectangle<int> visibleArea) = 0;

		JUCE_DECLARE_WEAK_REFERENCEABLE(Listener);
	};


	void visibleAreaChanged(const Rectangle<int>& newVisibleArea) override;
	void addListener(Listener* l) { listeners.addIfNotAlreadyThere(l); }
	void removeListener(Listener* l) { listeners.removeAllInstancesOf(l); }

	Array<WeakReference<Listener>> listeners;

	Rectangle<int> visibleArea;
};

/** a simple markdown parser that renders markdown formatted code. */
class MarkdownParser
{
public:
	
	static constexpr int DefaultLineWidth = 800;

	enum ResolveType
	{
		Fallback = 0, ///< creates a empty content
		FileBased, ///< creates the content from a file
		Autogenerated, ///< autogenerates the content
		WebBased, ///< creates the content from a web resource
		Cached, ///< creates the content from a cached database
		EmbeddedPath ///< Always generated
	};

	struct HyperLink
	{
		bool valid = false;
		Rectangle<float> area = {};

		MarkdownLink url = {};

		//String url = {}; // TODO_LINK: Use markdown link
		String tooltip;
		String displayString;
		Range<int> urlRange = {};
	};

	struct LinkResolver
	{
		struct Sorter
		{
			static int compareElements(LinkResolver* first, LinkResolver* second);
		};

		virtual ~LinkResolver() {};
		virtual String getContent(const MarkdownLink& url) = 0;
		virtual LinkResolver* clone(MarkdownParser* parent) const = 0;
		virtual Identifier getId() const = 0;
		
		virtual MarkdownLink resolveURL(const MarkdownLink& url) { return url; }

		virtual File getFileToEdit(const MarkdownLink& url) 
		{ 
			ignoreUnused(url);
			return {}; 
		};

		virtual ResolveType getPriority() const = 0;

		/** Overwrite this method and do something if the url matches, then return 
		    true or false whether the resolver consumed this event. 
		*/
		virtual bool linkWasClicked(const MarkdownLink& url) 
		{ 
			ignoreUnused(url);
			return false; 
		}
	};

	class ImageProvider
	{
	public:

		struct Sorter
		{
			static int compareElements(ImageProvider* first, ImageProvider* second);
		};

		bool operator==(const ImageProvider& other) const
		{
			return getId() == other.getId();
		}

		ImageProvider(MarkdownParser* parent_) :
			parent(parent_)
		{}

		virtual ResolveType getPriority() const { return ResolveType::Fallback; }

		virtual ~ImageProvider() {};

		/** Overwrite this method and return an image for the given URL.
		*
		*	The default function just returns a placeholder.
		*/
		virtual Image getImage(const MarkdownLink& /*imageURL*/, float width);
		virtual ImageProvider* clone(MarkdownParser* newParent) const { return new ImageProvider(newParent); }
		virtual Identifier getId() const { RETURN_STATIC_IDENTIFIER("EmptyImageProvider"); };

		static void updateWidthFromURL(const MarkdownLink& url, float& widthToUpdate);


		/** Helper function that makes sure that the image doesn't exceed the given width. */
		static Image resizeImageToFit(const Image& otherImage, float width);

	protected:
		MarkdownParser * parent;
	};

	class DefaultLinkResolver;	class FileLinkResolver;			class FolderTocCreator;
	class URLImageProvider;		class FileBasedImageProvider;	class GlobalPathProvider;
	class PathDescriptionResolver;
	
	MarkdownParser(const String& markdownCode);

	virtual ~MarkdownParser();

	void setFonts(Font normalFont, Font codeFont, Font headlineFont, float defaultFontSize);

	void setTextColour(Colour c) { styleData.textColour = c; }

	String resolveLink(const MarkdownLink& url);

	Image resolveImage(const MarkdownLink& imageUrl, float width);

	void setLinkResolver(LinkResolver* ownedResolver);

	void setImageProvider(ImageProvider* newProvider);;

	virtual void parse();

	Result getParseResult() const { return currentParseResult; }

	void setDefaultTextSize(float fontSize);

	void setDatabaseHolder(MarkdownDatabaseHolder* holder_);

	String getCurrentText(bool includeMarkdownHeader=true) const;

	MarkdownLink getLastLink() const;

	void setNewText(const String& newText);
	
	MarkdownLink getLinkForMouseEvent(const MouseEvent& event, Rectangle<float> whatArea);

	virtual void jumpToCurrentAnchor() {};

	virtual bool gotoLink(const MarkdownLink& url);

	void setLinkWithoutAction(const MarkdownLink& url)
	{
		lastLink = url;
	}

	int getLineNumberForY(float y) const;

	float getYForLineNumber(int lineNumber) const;

	Array<MarkdownLink> getImageLinks() const;

	HyperLink getHyperLinkForEvent(const MouseEvent& event, Rectangle<float> area);

	static void createDatabaseEntriesForFile(File root, MarkdownDataBase::Item& item, File f, Colour c);

	struct SnippetTokeniser : public CodeTokeniser
	{
		SnippetTokeniser() {};

		int readNextToken(CodeDocument::Iterator& source) override;
		CodeEditorComponent::ColourScheme getDefaultColourScheme();
	};

	struct TokeniserT
	{
		static int readNextToken(CodeDocument::Iterator& source);
	};

	
	struct Tokeniser: public CodeTokeniser
	{
		Tokeniser() {};

		int readNextToken(CodeDocument::Iterator& source);
		CodeEditorComponent::ColourScheme getDefaultColourScheme();
	};

	void clearResolvers()
	{
		imageProviders.clear();
		linkResolvers.clear();
	}

	void setCreateFooter(bool shouldCreateFooter)
	{
		createFooter = shouldCreateFooter;
	}

	void setStyleData(MarkdownLayout::StyleData newStyleData);

	MarkdownDatabaseHolder* getHolder() { return holder; }

	MarkdownLayout::StyleData& getStyleData() { return styleData; }
	
	MarkdownHeader getHeader() const { return header; }

	void clearCurrentLink()
	{
		lastLink = {};
	}

	bool hasLinks() const { return containsLinks; }

protected:

	struct Element
	{
		Element(MarkdownParser* parent_, int lineNumber_) :
			parent(parent_),
			lineNumber(lineNumber_)
		{
			hyperLinks.insertArray(0, parent->currentLinks.getRawDataPointer(), parent->currentLinks.size());
		};

		virtual ~Element() {};

		virtual void draw(Graphics& g, Rectangle<float> area) = 0;
		virtual float getHeightForWidth(float width) = 0;
		virtual int getTopMargin() const = 0;
		virtual Component* createComponent(int maxWidth)
		{ 
			ignoreUnused(maxWidth);
			return nullptr; 
		}
		
		float getZoomRatio() const { return parent->styleData.fontSize / 17.0f; }

		virtual String getTextForRange(Range<int> range) const;

		void drawHighlight(Graphics& g, Rectangle<float> area);;

		virtual void searchInContent(const String& s) 
		{
			ignoreUnused(s);
		}

		virtual void addImageLinks(Array<MarkdownLink>& sa) 
		{
			ignoreUnused(sa);
		};

		Array<HyperLink> hyperLinks;

		float getLastHeight()
		{
			return cachedHeight;
		}

		float getHeightForWidthCached(float width, bool forceUpdate=false);

		static void recalculateHyperLinkAreas(MarkdownLayout& l, Array<HyperLink>& links, float topMargin);

		void setSelected(bool isSelected)
		{
			selected = isSelected;
		}

		virtual String getTextToCopy() const = 0;

		bool selected = false;
		RectangleList<float> searchResults;

		virtual void prepareLinksForHtmlExport(const String& baseURL);

		int getLineNumber() const { return lineNumber; }

		String generateHtmlAndResolveLinks(const File& localRoot) const;

	protected:

		virtual String generateHtml() const { return {}; }

		static Array<Range<int>> getMatchRanges(const String& fullText, const String& searchString, bool countNewLines);

		void searchInStringInternal(const AttributedString& textToSearch, const String& searchString);

		MarkdownParser* parent;

		float lastWidth = -1.0f;
		float cachedHeight = 0.0f;
		int lineNumber;
	};

	struct TextBlock;	struct Headline;		struct BulletPointList;		struct Comment;
	struct CodeBlock;	struct MarkdownTable;	struct ImageElement;		struct EnumerationList;
	struct ActionButton; struct LiveCodeBlock;	struct ContentFooter;

	struct CellContent
	{
		bool isEmpty() const
		{
			return imageURL.isInvalid() && s.getText().isEmpty();
		}

		AttributedString s;

		MarkdownLink imageURL;
		Array<HyperLink> cellLinks;
	};

	using RowContent = Array<CellContent>;

	OwnedArray<Element> elements;

	String markdownCode;

private:

	bool containsLinks = false;

	bool createFooter = false;

	MarkdownDatabaseHolder* holder = nullptr;

	friend class JavascriptCodeEditor;
	
	MarkdownHeader header;

	MarkdownLink lastLink;

    class Iterator
	{
	public:

		Iterator(const String& text_);

		juce_wchar peek();
		bool advanceIfNotEOF(int numCharsToSkip = 1);
		bool advance(int numCharsToSkip = 1);
		bool advance(const String& stringToSkip);
		bool next(juce::juce_wchar& c);
		bool match(juce_wchar expected);
		bool matchIf(juce_wchar expected);
		void skipWhitespace();
		String getRestString() const;
		String advanceLine();

		int getLineNumber() const { return currentLine; }

	private:

		String text;
		CharPointer_UTF8 it;
		int currentLine = 0;
	};

	struct Helpers
	{
		static bool isNewElement(juce_wchar c);
		static bool isEndOfLine(juce_wchar c);
		static bool isNewToken(juce_wchar c, bool isCode);
		static bool belongsToTextBlock(juce_wchar c, bool isCode, bool stopAtLineEnd);
	};

	void resetForNewLine();

	void parseLine();
	void parseHeadline();
	void parseBulletList();
	void parseEnumeration();

	void parseText(bool stopAtEndOfLine=true);
	void parseMarkdownHeader();
	void parseBlock();
	void parseJavascriptBlock();
	void parseTable();
	void parseButton();
	void parseComment();
	ImageElement* parseImage();
	RowContent parseTableRow();
	bool isJavascriptBlock() const;
	bool isImageLink() const;
	void addCharacterToCurrentBlock(juce_wchar c);
	void resetCurrentBlock();
	void skipTagAndTrailingSpace();

	bool isBold = false;
	bool isItalic = false;
	bool isCode = false;

#if 0
	float defaultFontSize = 17.0f;
	Colour textColour = Colours::black;
	

	Font normalFont;
	Font boldFont;
	Font codeFont;
	Font headlineFont;
	
#endif
	
	Colour currentColour;
	Font currentFont;

	MarkdownLayout::StyleData styleData;
	
	Iterator it;
	Result currentParseResult;
	OwnedArray<ImageProvider> imageProviders;
	AttributedString currentlyParsedBlock;
	Array<HyperLink> currentLinks;
	
	OwnedArray<LinkResolver> linkResolvers;
	
	
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MarkdownParser);
	JUCE_DECLARE_WEAK_REFERENCEABLE(MarkdownParser);
};


struct ComponentWithDocumentation
{
	virtual ~ComponentWithDocumentation() {};

	virtual MarkdownLink getLink() const = 0;
};

class MarkdownHelpButton;

}