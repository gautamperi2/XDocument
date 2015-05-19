#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
///////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes               //
// Ver 1.3                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu		                            //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package uses C++11 constructs, most noteably std::shared_ptr. 
* The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* public interface:
*----------------------
* XmlDocument xDoc(argv[2], XmlDocument::sourceType::filename);
*	sPtr Node = makeTaggedElement("peri");
*	xDoc.GetElementsByTagName("title")[0]->addChild(Node);
*	xDoc.GetElementsByTagName("peri");
*	xDoc.toString();
*	xDoc.Save("created.xml");

*	XmlDocument yDoc;
*	sPtr root = makeTaggedElement("root");
*	root->addChild(makeTextElement("this is a test"));
*	sPtr child = makeTaggedElement("child");
*	child->addChild(makeTextElement("this is another test"));
*	child->addAttrib("first", "test");
*	root->addChild(child);
*	yDoc.AddRoot(root);
*	yDoc.toString();
*	yDoc.GetElementsByTagName("root")[0]->removeChild(yDoc.GetElementById("test"));
*	yDoc.toString();
*   sPtr root1 = yDoc.GetRoot();
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
*
* ver 1.3 29 March 2015
* - added functionality
*
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*
* ToDo:
* -----
* This is the beginning of an XmlDocument class for Project #2.
* It lays out a suggested design, which you are not compelled to follow.
* If it helps then use it.  If not you don't have to.
*
* Note that I've simply roughed in a design that is similar to that
* used in the .Net Framework XDocument class.
*/

#include <memory>
#include <string>
#include "../XmlElement/XmlElement.h"

namespace XmlProcessing
{
	class XmlDocument
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement > ;

		enum sourceType { string, filename };
		enum searchType { uniqueid, tagname };

		//copy, move constructors
		XmlDocument();
		XmlDocument(const std::string& src, sourceType srcType = string);		
		XmlDocument& operator=(XmlDocument&& doc);
		XmlDocument(XmlDocument&& doc);

		sPtr GetElementById(const std::string& tag);
		std::vector<sPtr> GetElementsByTagName(const std::string& tag);
		bool AddRoot(std::shared_ptr<AbstractXmlElement> pChild);
		bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		bool addAttribute(const std::string& key, const std::string& value);
		bool removeAttribute(const std::string& key); //remove attribute
		void toString();
		void Save(const std::string& filePath);	//save to file path
		sPtr GetRoot(){ return pDocElement_; }; //return xml root

	private:

		bool ParseParts();
		void PrepProcInstrElement();
		void PrepTaggedElement();
		void PrepTextElement();
		void PrepCommentElement();
		void PrepDeclElement();
		void PrepEndElement();
		void DepthFirstSearch(sPtr pNode, const std::string& tag, searchType type = tagname);

		sPtr pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
		std::vector<sPtr> found_;  // query results
		std::vector<std::string> toks;
		std::stack < std::shared_ptr<AbstractXmlElement> > treeStack;
		std::vector<std::shared_ptr<AbstractXmlElement>> treeFrame;
	};
}
#endif
