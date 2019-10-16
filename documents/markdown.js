function debug(text)
{
    document.writeln('<CODE CLASS=DEBUG>['+text+']</CODE>');    
}
function load_markdown(url)
{
    var r = new XMLHttpRequest()
    r.open('GET',url,false);
    r.send(null);
    if ( r.status == 200 )
    {
        var end = -1;
        var code = 0;
        var list = 0;
        for ( var start = 0 ; start >= 0 ; start = end )
        {
            end = r.responseText.indexOf('\n',start+1);
            if ( end < 0 )
            {
                break;
            }
            //debug(start+','+end);
            var line = r.responseText.substring(start,end);

            // newline -> P
            if ( line.substr(0,1) == '\n' )
            {
                if ( code == 0 )
                {
                    document.writeln('<P/>');
                }
                else
                {
                    document.writeln('<BR/>');
                }
                end = start+1;
                continue;
            }

            // ~~~ -> CODE
            if ( line.substr(0,3) == '~~~' )
            {
                if ( code == 0 )
                {
                    document.writeln('<CODE CLASS=BLOCK>');
                    code = 1;
                }
                else
                {
                    document.writeln('</CODE>');
                    code = 0;
                }
                continue;
            }

            // block code
            if ( code == 1 )
            {
                document.writeln(line);
                continue;
            }

            // markdowns
            if ( line.match("^ *[-\*]") != null )
            {
                if ( list == 2 )
                {
                    document.writeln('</OL>');
                    list = 0;
                }
                if ( list == 0 )
                {
                    document.writeln('<UL>');
                    list = 1;
                }
                line = '<LI>' + line.substring(1) + '</LI>';
            }
            else if ( line.match("^ *[0-9]+\.") != null )
            {
                if ( list == 1 )
                {
                    document.writeln('</UL>');
                    list = 0;
                }
                if ( list == 0 )
                {
                    document.writeln('<OL>');
                    list = 2;
                }
                line = '<LI>' + line.substring(1) + '</LI>';
            }
            else
            {
                if ( list == 1 )
                {
                    document.writeln('</UL>');
                }
                else if ( list == 2 )
                {
                    document.writeln('</OL>');
                }
                list = 0;
            }

            // #[#...] -> H
            var nh = 0;
            while ( line.substr(nh,nh+1) == '#' )
            {
                nh = nh + 1;
            }
            if ( nh > 0 )
            {
                document.write('<H'+nh+'>'+line.substr(nh+1)+'</H'+nh+'>');
                continue;
            }

            // https://... -> HREF
            var refs = line.match("https://[-A-Za-z_@.]+/[-A-Za-z_&+?/#.]+[-A-Za-z_&+?/]");
            if ( refs != null )
            {
                for ( var n = refs.length-1 ; n >= 0 ; n-- )
                {
                    line = line.replace(refs[n],'<A HREF="'+refs[n]+'">'+refs[n]+'</A>');
                }
            }

            // *...* -> I
            refs = line.match("\\*[-A-Za-z ]+\\*");
            if ( refs != null )
            {
                for ( var n = refs.length-1 ; n >= 0 ; n-- )
                {
                    var repl = refs[n].substring(1,refs[n].length-1);;
                    line = line.replace(refs[n],'<I>'+repl+'</I>');
                }
            }

            // _..._ -> I
            refs = line.match("_[-A-Za-z ]+_");
            if ( refs != null )
            {
                for ( var n = refs.length-1 ; n >= 0 ; n-- )
                {
                    var repl = refs[n].substring(1,refs[n].length-1);
                    line = line.replace(refs[n],'<I>'+repl+'</I>');
                }
            }

            // **...** -> B
            refs = line.match("\\*\\*[-A-Za-z ]+\\*\\*");
            if ( refs != null )
            {
                for ( var n = refs.length-1 ; n >= 0 ; n-- )
                {
                    var repl = refs[n].substring(2,refs[n].length-2);;
                    line = line.replace(refs[n],'<B>'+repl+'</B>');
                }
            }

            // __...__ -> B
            refs = line.match("__[-A-Za-z ]+__");
            if ( refs != null )
            {
                for ( var n = refs.length-1 ; n >= 0 ; n-- )
                {
                    var repl = refs[n].substring(2,italics[n].length-2);;
                    line = line.replace(refs[n],'<B>'+repl+'</B>');
                }
            }

            // *...* -> I
            refs = line.match("`.+`");
            if ( refs != null )
            {
                for ( var n = refs.length-1 ; n >= 0 ; n-- )
                {
                    var repl = refs[n].substring(1,refs[n].length-1);
                    line = line.replace(refs[n],'<CODE CLASS=INLINE>'+repl+'</CODE>');
                }
            }

            // just text -> text
            document.writeln(line);
        }
    }
}
