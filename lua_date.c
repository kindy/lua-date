#define DDEBUG 1
#include "ddebug.h"

#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR 102

static int days_between(lua_State *L);
static int days_diff(lua_State *L);
static int check(lua_State *L);
static int format(lua_State *L);
static int weekday(lua_State *L);

static const u_char dates[MAX_YEAR][12] =
{
    //from 2000 to 2101
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static const struct luaL_Reg date[] = {
    {"days_between", days_between},
    {"days_diff", days_diff},
    {"check", check},
    {"format", format},
    {"weekday", weekday},
    {NULL, NULL}
};


int
luaopen_date(lua_State *L)
{
    luaL_register(L, "date", date);
    return 1;
}


char * replace( char const * const original, char const * const pattern, char const * const replacement)
{
    size_t const replen = strlen(replacement);
    size_t const patlen = strlen(pattern);
    size_t const orilen = strlen(original);

    size_t patcnt = 0;
    const char * oriptr;
    const char * patloc;

    // find how many times the pattern occurs in the original string
    for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen) {
        patcnt++;
    }

    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
        // copy the original string,
        // replacing all the instances of the pattern
        char * retptr = returned;
        for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen) {
            size_t const skplen = patloc - oriptr;
            // copy the section until the occurence of the pattern
            strncpy(retptr, oriptr, skplen);
            retptr += skplen;
            // copy the replacement
            strncpy(retptr, replacement, replen);
            retptr += replen;
        }
        // copy the rest of the string.
        strcpy(retptr, oriptr);
    }
    return returned;
}


static int
check_date(const char *date)
{
    int y, m, d;

    y = atoi(date);
    m = atoi(date + 5);
    d = atoi(date + 8);

    if (y >= 2000 && y <= 2102 && m > 0 && m <= 12 && d <= dates[y-2000][m - 1] && d > 0) {
        return 0;
    }

    return 1;
}



static int
diff(const char *begin, const char *end)
{
    int num = 0;
    int i, j, y, m, d, y1, m1, d1;

    if (check_date(begin) || check_date(end)) {
        return 0;
    }

    y = atoi(begin);
    m = atoi(begin + 5);
    d = atoi(begin + 8);

    y1 = atoi(end);
    m1 = atoi(end + 5);
    d1 = atoi(end + 8);

    if (y1 < y || (y1 == y && m1 < m) || (y1 == y && m1 == m && d1 < d)) {
        return 0;
    }

    for (i = y; i <= y1; i++) {
        if (i < y1) {
            num += dates[i - 2000][m - 1] - d + 1;
            for (j = m + 1; j <= 12; j++) {
                num += dates[i - 2000][j - 1];
            }
            m = 1;

        } else {
            for (j = m; j < m1; j++) {
                num += dates[i - 2000][j - 1];
            }
            if (y == y1) {
                num = num + d1 -d + 1;
            } else {
                num = num + d1;
            }
        }
    }

    return num - 1;
}


static int
dayofweek(const char *date)
{
    int days = diff("2001-01-01", date);
    dd("diff_days: %d", days);
    return days % 7 + 1;
}


static int
format(lua_State *L)
{
    int              n;
    char            *format_date;
    char            *origin_date;
    size_t           len_origin_date;

    n = lua_gettop(L);

    if (n != 1) {
        return luaL_error(L, "expected 1 argument but got %d", n);
    }

    origin_date = (char *) luaL_checklstring(L, 1, &len_origin_date);

    if (len_origin_date != 10) {
        return 1;
    }

    format_date = replace(origin_date, "-", "");

    lua_pushlstring(L, format_date, sizeof("yyyyMMdd"));

    return 1;
}


static int
days_between(lua_State *L)
{
    int              n, i, num = 0, count = 1;
    int              y, m, d;
    char            *begin;
    char            *end;
    size_t           len_begin, len_end;
    char             buf[sizeof("2010-11-01")];

    n = lua_gettop(L);

    if (n != 2) {
        return luaL_error(L, "expected 2 argument but got %d", n);
    }

    begin = (char *) luaL_checklstring(L, 1, &len_begin);
    end   = (char *) luaL_checklstring(L, 2, &len_end);

    if (check_date(begin) || check_date(end)) {
        return 1;
    }

    if (len_begin != 10 || len_end != 10) {
        return 1;
    }

    num = diff(begin, end);
    lua_createtable(L, num, 0);

    y = atoi(begin);
    m = atoi(begin + 5);
    d = atoi(begin + 8);

    for (i = 0; i < num; i++) {
        if (d > dates[y - 2000][m - 1]) {
            d = 1;
            m++;
        }

        if (m > 12) {
            m = 1;
            d = 1;
            y++;
        }

        sprintf(buf, "%04d-%02d-%02d", y, m, d);
        lua_pushlstring(L, buf, 10);
        lua_rawseti(L, -2, count);

        count++;
        d++;
    }

    return 1;
}


static int
check(lua_State *L)
{
    int      n, res;
    size_t   len;
    char    *day;

    n = lua_gettop(L);

    if (n != 1) {
        return luaL_error(L, "expected 1 argument but got %d", n);
    }

    day = (char *) luaL_checklstring(L, 1, &len);
    if (len != 10) {
        return 1;
    }

    res = check_date(day);
    lua_pushinteger(L, res);

    return 1;
}


static int
days_diff(lua_State *L)
{
    int     n;
    size_t  len_begin, len_end;
    char   *begin, *end;

    n = lua_gettop(L);

    if (n != 2) {
        return luaL_error(L, "expected 2 argument but got %d", n);
    }

    begin = (char *) luaL_checklstring(L, 1, &len_begin);
    end   = (char *) luaL_checklstring(L, 2, &len_end);

    if (len_begin != 10 || len_end != 10) {
        return 1;
    }

    int num = diff(begin, end);

    if (num == 0) {
        return luaL_error(L, "wrong argument (1st argument %s < 2nd argument %s", begin, end);
    }

    lua_pushinteger(L, num);

    return 1;
}


static int
weekday(lua_State *L)
{
    int     n;
    int     week_index;
    size_t  len_date;
    char   *date;

    n = lua_gettop(L);
    if (n != 1) {
        return luaL_error(L, "expected 1 argument but got %d", n);
    }

    date = (char *) luaL_checklstring(L, 1, &len_date);
    dd("hello: %s", date);

    if (len_date != 10) {
        return 1;
    }

    week_index = dayofweek(date);

    if (week_index == 0) {
        lua_pushnil(L);
        return 1;
    }

    lua_pushinteger(L, week_index);

    return 1;
}
