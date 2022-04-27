/* -------------------------------------------------------------------------------
* Copyright (C) 2022 Six Impossible Things Before Breakfast Limited.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*	 * Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	 * Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in the
*	   documentation and/or other materials provided with the distribution.
*	 * Neither the name of the OLogN Technologies AG nor the
*	   names of its contributors may be used to endorse or promote products
*	   derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL OLogN Technologies AG BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* -------------------------------------------------------------------------------*/



using System;
using System.Diagnostics;
using System.Text;
using System.Threading;
using globalmq.marshalling;



class html_text_tag_converter
{
    /// <summary>
    /// NOT TESTED, ultra quick port from C++ not actually tested
    /// </summary>
    public static basic_test.HtmlTag htmlstring_to_htmltag(String html_string)
    {
        basic_test.HtmlTag htmltag = new basic_test.HtmlTag();
        htmltag.tags.setCurrentVariant(basic_test.HtmlTextOrTags_variants.taglists);


        while (html_string.Length > 0)
        {
            //std::string::size_type start, end;
            basic_test.HtmlTag newTag = new basic_test.HtmlTag();
            int start = html_string.IndexOf('<');
            if (start == -1)
            {
                htmltag.tags.setCurrentVariant(basic_test.HtmlTextOrTags_variants.text);
                htmltag.tags.str = html_string;
                return htmltag;
            }
            else if (start != 0)        //erase startoffile
            {
                string startoffile = html_string.Substring(0, start);
                basic_test.HtmlTag htmltag_before = htmlstring_to_htmltag(startoffile);
                htmltag.tags.tags.Add(htmltag_before);
                html_string = html_string.Substring(start);
                start = 0;
            }
            int end = html_string.IndexOf('>');


            string tagcontent = html_string.Substring(start + 1, end - 1 - start);

            string tagname;
            int i = tagcontent.IndexOf(' ');
            if (i != -1)
            {
                tagname = tagcontent.Substring(0, i);
                tagcontent = tagcontent.Substring(i);
            }
            else
            {
                tagname = tagcontent;
            }

            // fill the name and props
            htmltag.name = tagname;
            while (tagcontent.Length != 0)
            {
                int delim = tagcontent.IndexOf("=");
                string propname;
                string propval;
                if (delim != -1)
                {
                    propname = tagcontent.Substring(0, delim);
                    tagcontent = tagcontent.Substring(delim + 1);
                    delim = tagcontent.IndexOf("=");
                    if (delim == -1)
                    {
                        propval = tagcontent;
                    }
                    else
                    {
                        delim = tagcontent.IndexOf(" ");
                        propval = tagcontent.Substring(0, delim);
                        tagcontent.Substring(delim);
                    }
                    basic_test.Property p = new basic_test.Property();
                    p.name = propname;
                    p.value = propval;
                    htmltag.properties.Add(p);
                }
                else
                {
                    tagcontent = String.Empty;
                }
            }


            // erase close tag
            string closetag = "</" + tagname + ">";
            int lenght = closetag.Length;
            int closed = html_string.IndexOf(closetag);
            string lasttext;
            string textinside;
            string fronttext;
            int textinside_lenght = 0;
            if (closed != -1)               // if closed tag exist
            {
                textinside = html_string.Substring(end + 1, closed - (end + 1));
                textinside_lenght = textinside.Length;
                // erase in textinside the text after last close tag
                int counter1 = textinside.LastIndexOf('>');
                if (counter1 != -1)
                {
                    lasttext = textinside.Substring(counter1 + 1);
                    textinside = textinside.Substring(0, counter1 + 1);
                }

                // erase in textinside the text before firts opened tag
                int counter2 = textinside.IndexOf('<');
                if (counter2 != -1)
                {
                    fronttext = textinside.Substring(0, counter2);
                    textinside = textinside.Substring(counter2);
                }

                // get tag from textinside
                basic_test.HtmlTag htmltaginside = htmlstring_to_htmltag(textinside);
                htmltag.tags.tags.Add(htmltaginside);
                // erase text inside
                html_string = html_string.Substring(0, end + 1) + html_string.Substring(textinside_lenght);
                //---------------------------------------------------//

                // erase endoffile
                int closedpos = html_string.IndexOf(closetag);
                if (closedpos != -1)
                {
                    if ((closedpos + lenght) < html_string.Length)
                    {
                        string endoffile = html_string.Substring(closedpos + lenght);
                        basic_test.HtmlTag htmltag_after = htmlstring_to_htmltag(endoffile);
                        htmltag.tags.tags.Add(htmltag_after);
                        html_string = html_string.Substring(0, (closedpos + lenght));
                    }
                }


                // erase closed tag
                //html_string.erase(closedpos, lenght);
                html_string = html_string.Substring(0, closedpos) + html_string.Substring(closedpos + lenght);
            }



            // erase open tag
            //html_string.erase(start, end + 1);
            html_string = html_string.Substring(0, start) + html_string.Substring(end + 1);


        }

        return htmltag;
    }

    public static string htmltag_to_htmlstring(basic_test.IHtmlTag htmltag)
    {
        StringBuilder output = new StringBuilder();
        basic_test.HtmlTextOrTags_variants var = htmltag.tags.currentVariant();

        // add opened tag
        if (htmltag.name.Length != 0)
        {
            output.Append("<").Append(htmltag.name);

            if (htmltag.properties.Count != 0)
            {
                foreach (basic_test.IProperty prop in htmltag.properties)
                {
                    output.Append(prop.name).Append("=").Append(prop.value);
                }
            }
            output.Append(">");
        }

        // add inside content
        if (var == basic_test.HtmlTextOrTags_variants.taglists)
        {
            foreach (basic_test.IHtmlTag tag in htmltag.tags.tags)
            {
                //add text inside tag
                output.Append(htmltag_to_htmlstring(tag));
            }
        }
        else if (var == basic_test.HtmlTextOrTags_variants.text)
        {
            output.Append(htmltag.tags.str);
        }

        // add closed tag
        if (htmltag.name.Length != 0)
        {
            if (htmltag.name != "P") // condition for exceptions of singletags
            {
                output.Append("</").Append(htmltag.name).Append(">");
            }
        }


        return output.ToString();
    }

}
