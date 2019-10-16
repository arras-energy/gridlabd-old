function debug(text)
{
    document.writeln('<CODE CLASS=DEBUG>['+text+']</CODE>');    
}
function error(text)
{
    window.status = 'ERROR: ' + text;
}
function replace_tags(line,tag,pattern,key)
{
    while ( (refs=line.match(tag+pattern+tag)) != null )
    {
        var repl = refs[0].substring(tag.length,refs[0].length-tag.length);
        //debug(refs[0]+'->'+key+':'+repl);
        line = line.replace(refs[0],'<'+key+'>'+repl+'</'+key+'>');
    }    
    return line;
}
function write_markdown(markdown_text)
{
    if ( markdown_text != null )
    {
        var end = -1;
        var code = 0;
        var list = 0;
        for ( var start = 0 ; start >= 0 ; start = end )
        {
            end = markdown_text.indexOf('\n',start+1);
            if ( end < 0 )
            {
                break;
            }
            var line = markdown_text.substring(start,end);

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
            if ( line.substr(0,3) == '~~~' || line.substr(0,3) == '```')
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
            else if ( line.match("^ *1\\.") != null )
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
            if ( (ref=line.match("https?://[-A-Za-z_@.]+/[-A-Za-z_&+?/#.]+[-A-Za-z_&+?/]")) != null )
            {
                for ( n = ref.length-1 ; n >= 0; n-- )
                {
                    repl = '<A HREF="'+ref[n]+'">'+ref[n]+'</A>';
                    line = line.replace(ref[n],repl);
                }
            }

            // [[..]]... -> HREF
            if ( (ref=line.match("\[\[[A-Za-z0-9_ ]+\]\]")) != null )
            {
                for ( n = ref.length-1 ; n >= 0; n-- )
                {
                    var m = 2;
                    if ( ref[0].charAt(m) == '[' ) m++; // not sure why this is necessary
                    target = ref[0].substring(m,ref[0].length-2);
                    line = line.replace(ref[0],'<A HREF="'+root+'?page='+base+target+'.md">'+target+'</A>');
                }
            } 

            // *...* -> I
            line = replace_tags(line,'\\*','[-A-Za-z ]+','I');
            line = replace_tags(line,'_','[-A-Za-z ]+','I');
            line = replace_tags(line,'\\*\\*','[-A-Za-z ]+','B');
            line = replace_tags(line,'__','[-A-Za-z ]+','B');
            line = replace_tags(line,'`','.+','CODE');

            // just text -> text
            document.writeln(line);
        }
    }
    else
    {
        document.writeln("status = "+r.status);
    }
}

function load_markdown(base,page)
{
    root = base.substring(0,base.lastIndexOf('/')+1);
    url = root + page;
    if ( url.substring(0,7) == "file://" )
    {
        debug('access denied: ' + url + ' is local');
    }
    else
    {
        request = new XMLHttpRequest();
        debug('GET '+url);
        request.open('GET',url,true);
        request.onload = function ready() 
        {
            if ( request.status == 200 || request.status == 0 )
            {
                write_markdown(request.responseText);
            }
            else
            {
                error('error code '+request.status);
            }
        }
;
        request.send();
    }
}
