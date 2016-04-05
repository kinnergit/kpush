function split( str, delimiter )
    local t = {};

    string.gsub( str, delimiter .. "([_a-zA-Z0-9]+)", function( s )
        table.insert( t, s );
    end);

    return t;
end

function getXTableElement( filename, key )
    local tName = dofile( filename );

    local ret;
    local keyArray = split( key, "/" );

    for i = 1, #keyArray do
        local key = keyArray[i];
        local numberKey = tonumber( key );

        if numberKey then
            key = numberKey;
        end

        if i == 1 then
            ret = tName[key];
        else
            ret = ret[key];
        end
    end

    return ret;
end

function str_repeat( str, repeatNum )
    ret = "";

    repeatNum = tonumber( repeatNum );

    for i = 1, repeatNum do
        ret = ret .. str;
    end

    return ret;
end

function print_r ( ... )
    for _,v in ipairs( {...} ) do
        if type( v ) == "table" then
            _print_r( v );
        else
            print( v );
        end
    end
end

function _print_r( o, depth )
    if not depth then
        depth = 0;
    end

    if type( o ) == "number" then

        print( o );

    elseif type( o ) == "string" then

        print( string.format( "%q", o ) );

    elseif type( o ) == "boolean" then

        print( o );

    elseif type( o ) == "table" then

        local repeatStr = str_repeat( "  ", depth );
        local repeatStr1 = str_repeat( "  ", depth + 1 );

        print( repeatStr .. "{" );

        for k, v in pairs( o ) do
            if type( v ) == "table" then
                print( repeatStr1 .. "[" .. k .. "] => " );
                _print_r( v, depth + 1 );
            else
                print( repeatStr1 .. "[" .. k .. "]" .. " => " .. tostring( v ) );
            end
        end

        print( repeatStr .. "}" );

    end
end
