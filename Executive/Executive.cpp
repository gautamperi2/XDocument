///////////////////////////////////////////////////////////////////
// Executive.cpp - a container of XmlElement nodes               //
// Ver 1.0                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Asus Q550L, Win 8.1 Pro, Visual Studio 2013      //
// Author:      Gautam Nagesh Peri, CST 4-484, 3154846562        //
//              nperi@syr.edu		                            //
///////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "../XmlDocument/XmlDocument/XmlDocument.h"

using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement > ;

//Function to get user input
void GetUserChar()
{
	std::cout << "Press enter to continue...";
	getchar();
}

//Demo to accept file input and display xml
void demoPoint3_Files(const std::string file)
{
	std::cout << "\n  Processing file and display xml: " << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
		xDoc.toString();
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}

	GetUserChar();
}

//Demo to accept string input and display xml
void demoPoint3_strings(std::string sInput)
{
	std::cout << "\n  Processing strings in xml";
	std::cout << "\n  " << std::string(16, '-') << "\n\n";
	XmlDocument xDoc(sInput, XmlDocument::sourceType::string);
	if (xDoc.GetRoot() != nullptr)
		xDoc.toString();
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();
}

//Demo to display element by uniqueid
void demoPoint5_uniqueid(const std::string file, const std::string UniqueId)
{
	std::cout << "\n  Get Element By Id ";
	std::cout << "\n  " << std::string(16, '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		sPtr Node = xDoc.GetElementById(UniqueId);
		if (Node != nullptr)
			std::cout << " " << Node->toString() << "\n";
		else
		{
			std::cout << "\n \n";
			std::cout << "\n No elements found.\n";
		}
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}


	GetUserChar();
}

//Demo to display element by TagName
void demoPoint6_TagName(const std::string file, const std::string TagName)
{
	std::cout << "\n  Get Elements By Tag Name:\"" << TagName << "\"";
	std::cout << "\n  " << std::string(16, '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		std::vector<sPtr>& foundTags = xDoc.GetElementsByTagName(TagName);
		if (foundTags.size() != 0)
		{
			for (size_t index = 0; index < foundTags.size(); index++)
			{
				std::cout << " " << foundTags[index]->toString() << "\n";
			}
		}
		else
		{
			std::cout << "No Tags found." << "\n";
		}
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();

}

//Demo to display xml elements from empty tree
void demoPoint7_newTree()
{
	std::cout << "\n  Demo requirement7 to add child, new tree:";
	std::cout << "\n  " << std::string(16, '-') << "\n\n";

	XmlDocument yDoc;
	sPtr root = makeTaggedElement("root");
	root->addChild(makeTextElement("this is a test"));

	sPtr child = makeTaggedElement("child");
	child->addChild(makeTextElement("this is another test"));
	child->addAttrib("first", "test");
	root->addChild(child);

	yDoc.AddRoot(root);
	yDoc.toString();

	std::cout << "\n  Demo requirement7 to remove child, new tree:";
	std::cout << "\n  " << std::string(16, '-') << "\n\n";
	std::vector<sPtr>& foundTags = yDoc.GetElementsByTagName("root");
	if (foundTags.size() != 0)
	{
		foundTags[0]->removeChild(yDoc.GetElementById("test"));
	}
	else
	{
		std::cout << "No Tags found." << "\n";
	}
	yDoc.toString();

	GetUserChar();
}

//Demo to display xml attributes for a given element
void demoPoint8_attributes(const std::string file, const std::string TagName)
{
	std::cout << "\n  To Display attributes for a given tag: " << TagName << " in file : " << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		std::vector<sPtr>& foundTags = xDoc.GetElementsByTagName(TagName);

		if (foundTags.size() != 0)
		{
			std::vector<std::pair<std::string, std::string>> attribs = foundTags[0]->attributes();
			if (attribs.size() != 0)
			{
				std::cout << "key" << " : " << "value" << "\n";
				for (size_t index = 0; index < attribs.size(); index++)
				{
					std::cout << attribs[index].first << " : " << attribs[index].second << "\n";
				}
			}
			else
			{
				std::cout << "No attributes present." << "\n";
			}
		}
		else
		{
			std::cout << "No element present." << "\n";
		}
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();
}

//Demo to display xml children for a given element
void demoPoint8_children(const std::string file, const std::string TagName)
{
	std::cout << "\n  To Display children (descendants) for a given tag: " << TagName << " in file:" << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		std::vector<sPtr>& foundTags = xDoc.GetElementsByTagName(TagName);
		if (foundTags.size() != 0)
		{
			std::vector<sPtr> children = foundTags[0]->children();
			if (children.size() != 0)
			{
				for (size_t index = 0; index < children.size(); index++)
				{
					std::cout << " " << children[index]->toString() << "\n";

				}
			}
			else
			{
				std::cout << "No attributes present." << "\n";
			}
		}
		else
		{
			std::cout << "No element present." << "\n";
		}
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();
}

//Demo to add or remove attributes for a given element
void demoPoint9_addremoveAttribute(const std::string file, const std::string TagName, const std::string Key, const std::string Value)
{
	std::cout << "\n  To Display attributes for a given tag  in file:" << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		std::vector<sPtr>& foundTags = xDoc.GetElementsByTagName(TagName);
		if (foundTags.size() != 0)
		{
			bool IsAdded = foundTags[0]->addAttrib(Key, Value);
			std::cout << "attribute addition:" << IsAdded << "\n";
		}
		else
		{
			std::cout << "No elements  found to add or remove." << "\n";
		}
		//Demo remove attribute
		sPtr Node = xDoc.GetElementById("13210");
		if (Node != NULL)
		{
			Node->removeAttrib("zipcode");
		}
		else
		{
			std::cout << "No elements  found to add or remove." << "\n";
		}

		std::cout << "XML after removing attribute zipcode" << "\n";
		xDoc.toString();
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();

}

//Demo to add or remove children for a given element
void demoPoint9_addremoveChildren(const std::string file, const std::string TagName)
{
	std::cout << "\n  To Display addd/remove children for a given tag  in file:" << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		xDoc.toString();
		std::vector<sPtr>& foundTags = xDoc.GetElementsByTagName(TagName);
		sPtr Node = makeTaggedElement("peri");

		if (foundTags.size() != 0)
		{
			bool IsAdded = foundTags[0]->addChild(Node);
			std::cout << "Child addition:" << IsAdded << "\n";
			xDoc.toString();
		}
		else
		{
			std::cout << "No elements  found to add or remove." << "\n";
		}
		//Demo remove attribute
		foundTags = xDoc.GetElementsByTagName(TagName);
		if (Node != NULL)
		{
			foundTags = xDoc.GetElementsByTagName("author");
			sPtr node1 = xDoc.GetElementById("13210");
			if (foundTags.size() != 0)
			{
				bool IsRemoved = foundTags[0]->removeChild(node1);
				std::cout << "Child Removal: note zipcode 13210" << IsRemoved << "\n";
				xDoc.toString();
			}
		}
		else
		{
			std::cout << "No elements  found to add or remove." << "\n";
		}
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();

}

//Demo to save processed xml to a file.
void demoPoint10_SavetoFile(const std::string file)
{
	std::cout << "\n  To Save Xml after certain operations:" << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		sPtr Node = makeTaggedElement("peri");
		std::vector<sPtr>& foundTags = xDoc.GetElementsByTagName("title");
		if (foundTags.size() != 0)
		{
			foundTags[0]->addChild(Node);
		}

		sPtr Node1 = xDoc.GetElementById("13210");

		if (Node1 != nullptr)
		{
			Node1->addAttrib("state", "NY");
		}
		Node1 = xDoc.GetElementById("13210");

		if (Node1 != nullptr)
		{
			Node1->removeAttrib("Company");
		}
		foundTags = xDoc.GetElementsByTagName("author");
		if (foundTags.size() != 0)
		{
			Node1 = xDoc.GetElementById("SoftechSol");
			if (Node1 != nullptr)
				foundTags[0]->removeChild(Node1);
		}

		std::cout << "XML after doing multiple operations add/remove child and attribute" << "\n";
		std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";

		xDoc.Save("creation.xml");
		std::cout << "Xml file is saved as creation.xml in local folder" << "\n";
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();
}

//Demo to move constructors and move operators.
void demo_moveConstructor(const std::string file)
{
	std::cout << "\n  Move Constuctor Demo:" << file;
	std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
	XmlDocument xDoc(file, XmlDocument::sourceType::filename);
	if (xDoc.GetRoot() != nullptr)
	{
		xDoc.toString();

		XmlDocument yDoc = std::move(xDoc);
		yDoc.toString();

		std::cout << "xDoc (original):  " << &xDoc << "   yDoc (Moved)  " << &yDoc << "\n";

		std::cout << "\n  Move Operator Demo:";
		std::cout << "\n  " << std::string(16 + file.size(), '-') << "\n\n";
		XmlDocument zDoc;
		zDoc.toString();
		zDoc = std::move(yDoc);
		zDoc.toString();

		std::cout << "yDoc (original):  " << &yDoc << "   zDoc (Moved)  " << &zDoc << "\n";
	}
	else
	{
		std::cout << "Not a valid Xml." << "\n";
	}
	GetUserChar();
}


#ifdef TEST_EXECUTIVE
int main(int argc, char* argv[])
{
	try
	{
		if (argc < 3)
		{
			std::cout
				<< "\n  please enter name of file or xml string to process on command line\n\n";
			return 1;
		}
		if (!strcmp(argv[1], "file"))
		{
			demoPoint3_Files(argv[2]);
			demo_moveConstructor(argv[2]);
			demoPoint5_uniqueid(argv[2], "13210");
			demoPoint6_TagName(argv[2], "title");
			demoPoint7_newTree();
			demoPoint8_attributes(argv[2], "note");
			demoPoint8_children(argv[2], "reference");
			demoPoint9_addremoveAttribute(argv[2], "note", "state", "NY");
			demoPoint9_addremoveChildren(argv[2], "reference");
			demoPoint10_SavetoFile(argv[2]);
		}
		else if (!strcmp(argv[1], "string"))
		{
			std::string sInput = "<?xml version=\"1.0\" encoding=\"utf-8\"?><!-- XML test case --><LectureNote course=\"CSE681\">  <title>XML Example #1</title>  <reference>    <title>Programming Microsoft .Net</title>   <author>Jeff Prosise <note Company=\"Wintellect\" Street=\'Westcott\' zipcode='13210\'></note></author>    <publisher>Microsoft Press</publisher>    <date>2002</date>    <page>608</page>  </reference>  <title>Sample Element</title>  <title>Sample Element1</title> <title>Sample Element4</title>  <textelement Company=\'Wintellect\'/>  <comment>Description of CDATA</comment></LectureNote>";
			demoPoint3_strings(sInput);
		}
	}
	catch (int exception)
	{
		std::cout << "An exception occurred. Exception Nr. " << exception << '\n';
	}
}

#endif