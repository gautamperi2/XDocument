///////////////////////////////////////////////////////////////////
// XmlDocument.cpp - a container of XmlElement nodes             //
// Ver 1.3                                                       //
// Application: Pr#2, Spring 2015				                 //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013      //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu                                    //
///////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "XmlDocument.h"
#include "../../XmlElementParts/xmlElementParts.h"

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement > ;

//Constructor to create XmlDocument
XmlProcessing::XmlDocument::XmlDocument(const std::string& src, sourceType srcType)
{
	try
	{
		bool IsFile = false;

		if (srcType == XmlDocument::sourceType::filename)
			IsFile = true;
		else if (srcType == XmlDocument::sourceType::filename)
			IsFile = false;

		Toker toker(src, IsFile);
		toker.setMode(Toker::xml);
		XmlParts parts(&toker);
		while (parts.get())
		{
			toks = parts.GetTokens();
			ParseParts();
		}

		if (treeFrame.size() != 0 && treeStack.size() != 0)
		{
			pDocElement_ = makeDocElement(treeFrame, treeStack.top());
		}

	}
	catch (std::exception ex)
	{
		std::cout << "\n  " << ex.what() << "\n\n";
	}

}

//Constructor to create XmlDocument
XmlProcessing::XmlDocument::XmlDocument()
{
	pDocElement_ = makeDocElement(treeFrame);
}

//Parses and prepares Tagged xml element
void XmlDocument::PrepTaggedElement()
{

	sPtr Node = makeTaggedElement(toks[1]);
	bool IsInline = false;

	for (size_t i = 0; i < toks.size(); ++i)
	{
		if (toks[i] == "=")
		{
			std::string attValue = toks[i + 1];
			std::string x = attValue.substr(0, 1);
			std::string y = attValue.substr(attValue.size() - 1, 1);
			if ((x.compare("'") == 0 || x.compare("\"") == 0) && (y.compare("'") == 0 || y.compare("\"") == 0))
			{
				attValue = attValue.substr(1, attValue.size() - 2);
			}


			Node->addAttrib(toks[i - 1], attValue);
		}

		if (toks[i] == "/")
			IsInline = true;
	}
	if (!treeStack.empty())
		treeStack.top()->addChild(Node);

	if (!IsInline)
		treeStack.push(Node);

}

//Parses and prepares Text xml element
void XmlDocument::PrepTextElement()
{
	std::string text;

	for (size_t i = 0; i < toks.size(); ++i)
	{
		text = text + toks[i] + " ";
	}
	//need to trim the last space
	//text.trim();

	sPtr Node = makeTextElement(text);

	if (!treeStack.empty())
		treeStack.top()->addChild(Node);

}

//Parses and prepares comment xml element
void XmlDocument::PrepCommentElement()
{
	std::string text;

	for (size_t i = 2; i < toks.size(); ++i)
	{
		if (toks[i] != "--" && toks[i] != ">")
			text = text + toks[i] + " ";
	}

	//need to trim the last space
	//text.trim();

	sPtr Node = makeCommentElement(text);

	if (!treeStack.empty())
		treeStack.top()->addChild(Node);
	else
	{
		if (treeFrame.size() != 0)
			treeFrame.push_back(Node);
	}




}

//Parses and prepares declaration element
void XmlDocument::PrepDeclElement()
{
	sPtr Node = makeXmlDeclarElement();

	for (size_t i = 0; i < toks.size(); ++i)
	{
		if (toks[i] == "=")
		{
			std::string attValue = toks[i + 1];
			std::string x = attValue.substr(0, 1);
			std::string y = attValue.substr(attValue.size() - 1, 1);
			if ((x.compare("'") == 0 || x.compare("\"") == 0) && (y.compare("'") == 0 || y.compare("\"") == 0))
			{
				attValue = attValue.substr(1, attValue.size() - 2);
			}

			Node->addAttrib(toks[i - 1], attValue);
		}

	}

	treeFrame.push_back(Node);
}

//Parses and prepares comment declaration element
void XmlDocument::PrepEndElement()
{
	if (treeStack.size() != 1)
	{
		sPtr Node = treeStack.top();
		treeStack.pop();

		/*sPtr docEl = treeFrame.front();
		std::cout << "  " << docEl->toString();
		std::cout << "  " << Node->toString();
		std::cout << "\n\n";*/
	}
}

//Parses and prepares the xml 
bool XmlDocument::ParseParts()
{
	if (toks.size() == 0)
		return false;

	if (toks[0] == "<")
	{
		if (toks[1] == "?")
		{
			//Declaration Element			
			PrepDeclElement();
		}
		else if (toks[1] == "!")
		{
			if (toks[2] == "--")
			{
				PrepCommentElement();
			}
			else
			{
				//ProcInstrElemet
				//std::cout << " Proc INstr Element";

			}
		}
		else if (toks[1] == "/")
		{
			PrepEndElement();

		}
		else
		{
			PrepTaggedElement();
		}

	}
	else
	{
		PrepTextElement();
	}

	return true;

}

//performs DFS for the Xml processing 
void XmlDocument::DepthFirstSearch(sPtr pNode, const std::string& tag, searchType type)
{
	if (type == XmlDocument::searchType::tagname)
	{
		if (tag.compare(pNode->value()) == 0)
			found_.push_back(pNode);
	}
	else if (type == XmlDocument::searchType::uniqueid)
	{
		std::vector<std::pair<std::string, std::string>> attribs = pNode->attributes();
		for (size_t index = 0; index < attribs.size(); index++)
		{
			if (attribs[index].second.compare(tag) == 0)
				found_.push_back(pNode);
		}
	}
	std::vector<std::shared_ptr<AbstractXmlElement>> children = pNode->children();
	for (size_t index = 0; index < children.size(); index++)
	{
		DepthFirstSearch(children[index], tag, type);
	}
}

//Gets element by unique ID
sPtr XmlDocument::GetElementById(const std::string& tag)
{
	found_.clear();
	DepthFirstSearch(pDocElement_, tag, XmlDocument::searchType::uniqueid);
	if (found_.size()>0)
		return found_[0];
	else
		return nullptr;
}

//Gets elements by TagName
std::vector<sPtr> XmlDocument::GetElementsByTagName(const std::string& tag)
{
	found_.clear();
	DepthFirstSearch(pDocElement_, tag, XmlDocument::searchType::tagname);
	return found_;
}

//Displays xml document
void XmlDocument::toString()
{
	std::cout << "  " << pDocElement_->toString();
	std::cout << "\n\n";
}

//Adds root to Xml tree frame
bool XmlDocument::AddRoot(std::shared_ptr<AbstractXmlElement> pChild)
{
	pDocElement_->addChild(pChild);
	return true;
}

//Saves the processed Xml to a file
void XmlDocument::Save(const std::string& filePath)
{
	std::ofstream myfile;
	myfile.open(filePath);
	myfile << pDocElement_->toString();
	myfile.close();
}

//Move operator = to move xml from one to another.
XmlProcessing::XmlDocument& XmlDocument::operator = (XmlDocument&& doc)
{
	if (this != &doc)
	{
		pDocElement_ = NULL;         // AST that holds procInstr, comments, XML root, and more comments

		found_.clear();
		toks.clear();
		while (!treeStack.empty())
			treeStack.pop();

		treeFrame.clear();

		pDocElement_ = doc.pDocElement_;
		found_ = doc.found_;
		treeStack = doc.treeStack;
		treeFrame = doc.treeFrame;

		doc.pDocElement_ = NULL;

		doc.found_.clear();
		doc.toks.clear();
		while (!doc.treeStack.empty())
			doc.treeStack.pop();

		doc.treeFrame.clear();
	}

	return *this;
}

//Move constructor to move xml from one to another.
XmlDocument::XmlDocument(XmlDocument&& doc)
{
	pDocElement_ = doc.pDocElement_;
	found_ = doc.found_;
	treeStack = doc.treeStack;
	treeFrame = doc.treeFrame;

	doc.pDocElement_ = NULL;

	doc.found_.clear();
	doc.toks.clear();
	while (!doc.treeStack.empty())
		doc.treeStack.pop();

	doc.treeFrame.clear();

}

#ifdef TEST_XMLDOCUMENT 

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing xmldocument class\n "
		<< std::string(23, '=') << std::endl;
	std::cout
		<< "\n  Note that quotes are returned as single tokens\n\n";

	if (argc < 3)
	{
		std::cout
			<< "\n  please enter name of file or xml string to process on command line\n\n";
		return 1;
	}

	if (!strcmp(argv[1], "file"))
	{
		std::cout << "\n  Processing file " << argv[2];
		std::cout << "\n  " << std::string(16 + strlen(argv[2]), '-') << "\n\n";
		XmlDocument xDoc(argv[2], XmlDocument::sourceType::filename);
		sPtr Node = makeTaggedElement("peri");
		xDoc.GetElementsByTagName("title")[0]->addChild(Node);
		xDoc.GetElementsByTagName("peri");
		xDoc.toString();
		xDoc.Save("created.xml");
		//NEW TREEE
		XmlDocument yDoc;
		sPtr root = makeTaggedElement("root");
		root->addChild(makeTextElement("this is a test"));
		sPtr child = makeTaggedElement("child");
		child->addChild(makeTextElement("this is another test"));
		child->addAttrib("first", "test");
		root->addChild(child);
		yDoc.AddRoot(root);
		yDoc.toString();
		yDoc.GetElementsByTagName("root")[0]->removeChild(yDoc.GetElementById("test"));
		yDoc.toString();
	}
	else if (!strcmp(argv[1], "string"))
	{
		std::cout << "\n  Processing string " << argv[2];
		std::cout << "\n  " << std::string(16 + strlen(argv[2]), '-') << "\n\n";
		std::string sInput = "<?xml version=\"1.0\" encoding=\"utf-8\"?><!-- XML test case --><LectureNote course=\"CSE681\">  <title>XML Example #1</title>  <reference>    <title>Programming Microsoft .Net</title>   <author>Jeff Prosise <note Company=\"Wintellect\" Street=\'Westcott\' zipcode='13210\'></note></author>    <publisher>Microsoft Press</publisher>    <date>2002</date>    <page>608</page>  </reference>  <title>Sample Element</title>  <title>Sample Element1</title> <title>Sample Element4</title>  <textelement Company=\'Wintellect\'/>  <comment>Description of CDATA</comment></LectureNote>";		
		XmlDocument xDoc(sInput, XmlDocument::sourceType::string);
		xDoc.toString();
	}
}

#endif