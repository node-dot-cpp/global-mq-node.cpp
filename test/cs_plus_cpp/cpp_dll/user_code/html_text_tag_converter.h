// html_text_tag_converter.h

#ifndef HTML_TEXT_TAG_CONVERTER_H
#define HTML_TEXT_TAG_CONVERTER_H

#include <nodecpp/common.h>
#include "generated/publishable_state.h"


basic_test::structures::HtmlTag htmlstring_to_htmltag(std::string html_string)
{
    basic_test::structures::HtmlTag htmltag;
    htmltag.tags.initAs(basic_test::structures::HtmlTextOrTags::Variants::taglists);

    while (html_string.length() > 0)
    {
        std::string::size_type start, end;
        basic_test::structures::HtmlTag newTag;
        start = html_string.find('<');
        if (start == std::string::npos)
        {
            htmltag.tags.initAs(basic_test::structures::HtmlTextOrTags::Variants::text);
            htmltag.tags.str() = html_string;
            return htmltag;
        } 
        else if (start != 0)        //erase startoffile
        {
            std::string startoffile = html_string.substr(0, start);
            basic_test::structures::HtmlTag htmltag_before = htmlstring_to_htmltag(startoffile);
            htmltag.tags.tags().push_back(htmltag_before);
            html_string.erase(0, start);
            start = 0;
        }
        end = html_string.find('>');
        std::string tagcontent;


        tagcontent = html_string.substr(start + 1, end - 1 - start);

        std::string tagname;
        std::string::size_type i = tagcontent.find(' ');
        if (i != std::string::npos)
        {
            tagname = tagcontent.substr(0, i);
            tagcontent = tagcontent.substr(i);
        }
        else
        {
            tagname = tagcontent;
        }

        // fill the name and props
        htmltag.name = tagname;
        while (!tagcontent.empty())
        {
            std::string::size_type delim = tagcontent.find("=");
            std::string propname, propval;
            if (delim != std::string::npos)
            {
                propname = tagcontent.substr(0, delim);
                tagcontent = tagcontent.substr(delim + 1);
                delim = tagcontent.find("=");
                if (delim == std::string::npos)
                {
                    propval = tagcontent;
                }
                else
                {
                    delim = tagcontent.find(" ");
                    assert(delim != std::string::npos);
                    propval = tagcontent.substr(0, delim);
                    tagcontent.substr(delim);
                }
                htmltag.properties.push_back({propname, propval});
            }
            else
            {
                tagcontent.clear();
            }
        }


        // erase close tag
        std::string closetag = "</";
        closetag += tagname + ">";
        int lenght = closetag.length();
        std::string::size_type closed = html_string.find(closetag);
        std::string lasttext, textinside, fronttext;
        int textinside_lenght = 0;
        if (closed != std::string::npos)               // if closed tag exist
        {
            textinside = html_string.substr(end + 1,  closed - (end +1));
            textinside_lenght = textinside.length();
            // erase in textinside the text after last close tag
            std::string::size_type counter1 = textinside.rfind('>');
            if (counter1 != std::string::npos)
            {
                lasttext = textinside.substr(counter1 + 1);
                textinside.erase(counter1 + 1);
            }

            // erase in textinside the text before firts opened tag
            std::string::size_type counter2 = textinside.find('<');
            if (counter2 != std::string::npos)
            {
                fronttext = textinside.substr(0, counter2);
                textinside.erase(0, counter2);
            }

            // get tag from textinside
            basic_test::structures::HtmlTag htmltaginside = htmlstring_to_htmltag(textinside);
            htmltag.tags.tags().push_back(htmltaginside);
            // erase text inside
            html_string.erase(end + 1, textinside_lenght);
        //---------------------------------------------------//

            // erase endoffile
            std::string::size_type closedpos = html_string.find(closetag);
            if (closedpos != std::string::npos)
            {
                if ((closedpos + lenght) < html_string.length())
                {
                    std::string endoffile = html_string.substr(closedpos + lenght);
                    basic_test::structures::HtmlTag htmltag_after = htmlstring_to_htmltag(endoffile);
                    htmltag.tags.tags().push_back(htmltag_after);
                    html_string.erase((closedpos + lenght), html_string.length());
                }
            }
            

            // erase closed tag
            html_string.erase(closedpos, lenght);
        }

        

        // erase open tag
        html_string.erase(start, end + 1);

        
    }

    return htmltag;
}

std::string htmltag_to_htmlstring(basic_test::structures::HtmlTag htmltag)
{
    std::string output;
    basic_test::structures::HtmlTextOrTags::Variants var = htmltag.tags.currentVariant();

    // add opened tag
    if (!htmltag.name.empty())
    {
        output += "<" + htmltag.name;

        if (!htmltag.properties.empty())
        {
            for (auto prop : htmltag.properties)
            {
                output += prop.name + "=" + prop.value;
            }
        }
        output += ">";
    }

    // add inside content
    if (var == basic_test::structures::HtmlTextOrTags::Variants::taglists)
    {
        for (auto tag : htmltag.tags.tags())
        {
            //add text inside tag
            output += htmltag_to_htmlstring(tag);
        }
    }
    else if (var == basic_test::structures::HtmlTextOrTags::Variants::text)
    {
        output += htmltag.tags.str();
    }

    // add closed tag
    if (!htmltag.name.empty())
    {
        if (htmltag.name != "P") // condition for exceptions of singletags
        {
            output += "</" + htmltag.name + ">";
        }
    }


    return output;
}

#endif // HTML_TEXT_TAG_CONVERTER_H
