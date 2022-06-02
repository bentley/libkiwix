
#define CPPHTTPLIB_ZLIB_SUPPORT 1
#include "./httplib.h"
#include "gtest/gtest.h"

#define SERVER_PORT 8101
#include "server_testing_tools.h"

std::string makeSearchResultsHtml(const std::string& pattern,
                                  const std::string& header,
                                  const std::string& results,
                                  const std::string& footer)
{
  const char SEARCHRESULTS_HTML_TEMPLATE[] = R"HTML(<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="content-type" />

    //EOLWHITESPACEMARKER
    <style type="text/css">
      body{
      color: #000000;
      font: small/normal Arial,Helvetica,Sans-Serif;
      margin-top: 0.5em;
      font-size: 90%;
      }

      a{
      color: #04c;
      }

      a:visited {
      color: #639
      }

      a:hover {
      text-decoration: underline
      }

      .header {
      font-size: 120%;
      }

      ul {
      margin:0;
      padding:0
      }

      .results {
      font-size: 110%;
      }

      .results li {
      list-style-type:none;
      margin-top: 0.5em;
      }

      .results a {
      font-size: 110%;
      text-decoration: underline
      }

      cite {
      font-style:normal;
      word-wrap:break-word;
      display: block;
      font-size: 100%;
      }

      .informations {
      color: #388222;
      font-size: 100%;
      }

      .book-title {
      color: #662200;
      font-size: 100%;
      }

      .footer {
      padding: 0;
      margin-top: 1em;
      width: 100%;
      float: left
      }

      .footer a, .footer span {
      display: block;
      padding: .3em .7em;
      margin: 0 .38em 0 0;
      text-align:center;
      text-decoration: none;
      }

      .footer a:hover {
      background: #ededed;
      }

      .footer ul, .footer li {
      list-style:none;
      margin: 0;
      padding: 0;
      }

      .footer li {
      float: left;
      }

      .selected {
      background: #ededed;
      }

    </style>
    <title>Search: %PATTERN%</title>
  <link type="root" href="/ROOT"></head>
  <body bgcolor="white">
    <div class="header">
      %HEADER%
    </div>

    <div class="results">
      <ul>%RESULTS%
      </ul>
    </div>

    <div class="footer">%FOOTER%
    </div>
  </body>
</html>
)HTML";

  std::string html = removeEOLWhitespaceMarkers(SEARCHRESULTS_HTML_TEMPLATE);
  html = replace(html, "%PATTERN%", pattern);
  html = replace(html, "%HEADER%", header);
  html = replace(html, "%RESULTS%", results);
  html = replace(html, "%FOOTER%", footer);
  return html;
}

const std::vector<std::string> LARGE_SEARCH_RESULTS = {
R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Genius_+_Soul_=_Jazz">
              Genius + Soul = Jazz
            </a>
              <cite>...Grammy Hall of Fame in 2011. It was re-issued in the UK, first in 1989 on the Castle Communications "Essential Records" label, and by Rhino Records in 1997 on a single CD together with Charles' 1970 My Kind of <b>Jazz</b>. In 2010, Concord Records released a deluxe edition comprising digitally remastered versions of Genius + Soul = <b>Jazz</b>, My Kind of <b>Jazz</b>, <b>Jazz</b> Number II, and My Kind of <b>Jazz</b> Part 3. Professional ratings Review scores Source Rating Allmusic link Warr.org link Encyclopedia of Popular Music...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">242 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Jazz_Number_II">
              Jazz Number II
            </a>
              <cite><b>Jazz</b> Number II <b>Jazz</b> Number II is a 1973 album by Ray Charles. It is a collection of <b>jazz</b>/soul instrumentals featuring Charles on piano backed by his Big Band. Professional ratings Review scores Source Rating Allmusic link <b>Jazz</b> Number II Studio album by Ray Charles Released January 1973 Recorded 1971-72 Studio Charles’ Tangerine/RPM Studios, Los Angeles, CA Genre Soul, <b>jazz</b> Length 39:02 Label Tangerine Producer Ray Charles Ray Charles chronology Through the Eyes of Love (1972) <b>Jazz</b> Number II......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">87 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/My_Kind_of_Jazz_Part_3">
              My Kind of Jazz Part 3
            </a>
              <cite>My Kind of <b>Jazz</b> Part 3 My Kind of <b>Jazz</b> Part 3 is a 1975 album by Ray Charles released by Crossover Records. Concord Records re-issued the contents in digital form in 2009. Professional ratings Review scores Source Rating Allmusic link My Kind of <b>Jazz</b> Part 3 Studio album by Ray Charles Released October 1975 Recorded 1975 in Los Angeles, CA Genre Soul, <b>jazz</b> Length 38:13 Label Crossover Producer Ray Charles Ray Charles chronology Renaissance (1975) My Kind of <b>Jazz</b> Part 3 (1975) Live In Japan (1975)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">88 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/My_Kind_of_Jazz">
              My Kind of Jazz
            </a>
              <cite>My Kind of <b>Jazz</b> My Kind of <b>Jazz</b> Studio album by Ray Charles Released April 1970 Recorded January 1-10, 1970 in Los Angeles, CA Genre <b>jazz</b> Length 30:20 Label Tangerine Producer Quincy Jones Ray Charles chronology Doing His Thing (1969) My Kind of <b>Jazz</b> (1970) Love Country Style (1970) Professional ratings Review scores Source Rating Allmusic link My Kind of <b>Jazz</b> is a 1970 album by Ray Charles....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">69 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Hank_Crawford">
              Hank Crawford
            </a>
              <cite>...bop, <b>jazz</b>-funk, soul <b>jazz</b> alto saxophonist, arranger and songwriter. Crawford was musical director for Ray Charles before embarking on a solo career releasing many well-regarded albums on Atlantic, CTI and Milestone. Hank Crawford Background information Birth name Bennie Ross Crawford, Jr Born (1934-12-21)December 21, 1934 Memphis, Tennessee, U.S. Died January 29, 2009(2009-01-29) (aged 74) Memphis, Tennessee, U.S. Genres R&amp;B, Hard bop, <b>Jazz</b>-funk, Soul <b>jazz</b> Occupation(s) Saxophonist, Songwriter......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">102 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Catchin'_Some_Rays:_The_Music_of_Ray_Charles">
              Catchin&apos; Some Rays: The Music of Ray Charles
            </a>
              <cite>...<b>jazz</b> singer Roseanna Vitro, released in August 1997 on the Telarc <b>Jazz</b> label. Catchin' Some Rays: The Music of Ray Charles Studio album by Roseanna Vitro Released August 1997 Recorded March 26, 1997 at Sound on Sound, NYC April 4,1997 at Quad Recording Studios, NYC Genre Vocal <b>jazz</b> Length 61:00 Label Telarc <b>Jazz</b> CD-83419 Producer Paul Wickliffe Roseanna Vitro chronology Passion Dance (1996) Catchin' Some Rays: The Music of Ray Charles (1997) The Time of My Life: Roseanna Vitro Sings the Songs of......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">118 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/That's_What_I_Say:_John_Scofield_Plays_the_Music_of_Ray_Charles">
              That&apos;s What I Say: John Scofield Plays the Music of Ray Charles
            </a>
              <cite>That's What I Say: John Scofield Plays the Music of Ray Charles Studio album by John Scofield Released June 7, 2005 (2005-06-07) Recorded December 2004 Studio Avatar Studios, New York City Genre <b>Jazz</b> Length 65:21 Label Verve Producer Steve Jordan John Scofield chronology EnRoute: John Scofield Trio LIVE (2004) That's What I Say: John Scofield Plays the Music of Ray Charles (2005) Out Louder (2006) Professional ratings Review scores Source Rating Allmusic All About <b>Jazz</b> All About <b>Jazz</b>...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">109 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Tribute_to_Uncle_Ray">
              Tribute to Uncle Ray
            </a>
              <cite>...Stevie Wonder" with the successful and popular Ray Charles who was also a blind African American musician. Like his debut, this album failed to generate hit singles as Motown struggled to find a sound to fit Wonder, who was just 12 when this album was released. Tribute to Uncle Ray Studio album by Little Stevie Wonder Released October 1962 Recorded 1962 Studio Studio A, Hitsville USA, Detroit Genre Soul, <b>jazz</b> Label Tamla Producer Henry Cosby, Clarence Paul Stevie Wonder chronology The <b>Jazz</b> Soul of Little Stevie (1962) Tribute to Uncle Ray (1962) Recorded Live: The 12 Year Old Genius (1963) Professional ratings Review scores Source Rating Allmusic...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">165 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Best_of_Ray_Charles">
              The Best of Ray Charles
            </a>
              <cite>The Best of Ray Charles The Best of Ray Charles is a compilation album released in 1970 on the Atlantic <b>Jazz</b> label, featuring previously released instrumental (non-vocal) tracks recorded by Ray Charles between November 1956 and November 1958. The Best of Ray Charles Greatest hits album by Ray Charles Released 1970 Genre R&amp;B, <b>Jazz</b> Length 34:06 Label Atlantic The instrumental, "Rockhouse" would later be covered, as "Ray's Rockhouse" (1985), by The Manhattan Transfer with lyrics by Jon Hendricks....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">79 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Genius_Hits_the_Road">
              The Genius Hits the Road
            </a>
              <cite>...a hit single, "Georgia on My Mind". The Genius Hits the Road Studio album by Ray Charles Released September 1960 Recorded March 25 and 29, 1960 in New York City Genre R&amp;B, blues, <b>jazz</b> Length 33:37 Label ABC-Paramount 335 Producer Sid Feller Ray Charles chronology Genius + Soul = <b>Jazz</b> (1961) The Genius Hits the Road (1960) Dedicated to You (1961) Singles from The Genius Hits the Road "Georgia on My Mind" Released: September 1960 Professional ratings Review scores Source Rating Allmusic Warr.org...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">127 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Charles_at_Newport">
              Ray Charles at Newport
            </a>
              <cite>...Ray Charles at Newport is a 1958 live album of Ray Charles' July 5, 1958 performance at the Newport <b>Jazz</b> Festival. The detailed liner notes on the album were written by Kenneth Lee Karpe. All tracks from this Newport album, along with all tracks from his 1959 Herndon Stadium performance in Atlanta, were also released on the Atlantic compilation LP, Ray Charles Live. A later CD reissue of that compilation album included a previously unissued song from the 1958 Newport concert, "Swanee River Rock". Professional ratings Review scores Source Rating Allmusic link Discogs link Ray Charles at Newport Live album by Ray Charles Released November 1958 Recorded July 5, 1958 Venue Newport <b>Jazz</b> Festival, Newport, Rhode Island Genre R&amp;B Length 40:28 Label Atlantic Producer Tom Dowd (engineer) Ray Charles chronology The Great Ray Charles (1957) Ray Charles at Newport (1958) Yes Indeed! (1958) Re-issue cover 1987 re-issue/compilation...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">152 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Here_We_Go_Again:_Celebrating_the_Genius_of_Ray_Charles">
              Here We Go Again: Celebrating the Genius of Ray Charles
            </a>
              <cite>...and <b>jazz</b> trumpeter Wynton Marsalis. It was recorded during concerts at the Rose Theater in New York City, on February 9 and 10, 2009. The album received mixed reviews, in which the instrumentation of Marsalis' orchestra was praised by the critics. Here We Go Again: Celebrating the Genius of Ray Charles Live album by Willie Nelson and Wynton Marsalis Released March 29, 2011 (2011-03-29) Recorded February 9 –10 2009 Venue Rose Theater, New York Genre <b>Jazz</b>, country Length 61:49 Label Blue Note......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">167 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Confession_Blues">
              Confession Blues
            </a>
              <cite>...<b>jazz</b> Length 2:31 Label Down Beat Records Songwriter(s) R. C. Robinson (Ray Charles) Charles moved to Seattle in 1948, where he formed The McSon Trio with guitarist G. D. "Gossie" McKee and bass player Milton S. Garret. In late 1948, Jack Lauderdale of Down Beat Records heard Charles play at the Seattle <b>jazz</b> club, The Rocking Chair. The next day, Lauderdale took Charles and his trio to a Seattle recording studio where they recorded "Confession Blues" and "I Love You, I Love You". In February......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">284 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Genius_Loves_Company">
              Genius Loves Company
            </a>
              <cite>...<b>jazz</b> and pop standards performed by Charles and several guest musicians, such as Natalie Cole, Elton John, James Taylor, Norah Jones, B.B. King, Gladys Knight, Diana Krall, Van Morrison, Willie Nelson and Bonnie Raitt. Genius Loves Company was the last album recorded and completed by Charles before his death in June 2004. Genius Loves Company Studio album by Ray Charles Released August 31, 2004 Recorded June 2003–March 2004 Genre Rhythm and blues, soul, country, blues, <b>jazz</b>, pop Length 54:03......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">325 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Love_Country_Style">
              Love Country Style
            </a>
              <cite>Love Country Style Love Country Style is a studio album by Ray Charles released in June 1970 on Charles' Tangerine Records label. Love Country Style Studio album by Ray Charles Released June 1970 Genre R&amp;B Length 35:25 Label ABC/Tangerine Producer Joe Adams Ray Charles chronology My Kind of <b>Jazz</b> (1970) Love Country Style (1970) Volcanic Action of My Soul (1971) Professional ratings Review scores Source Rating Allmusic Christgau's Record Guide B...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">72 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Doing_His_Thing">
              Doing His Thing
            </a>
              <cite>Doing His Thing Doing His Thing is a 1969 studio album by Ray Charles, released by Tangerine Records. The cover artwork was by Lafayette Chew. Doing His Thing Studio album by Ray Charles Released May 1969 Recorded RPM Studios, Los Angeles, California Genre R&amp;B, soul Length 32:33 Label ABC/Tangerine Producer Joe Adams Ray Charles chronology I'm All Yours Baby (1969) Doing His Thing (1969) My Kind of <b>Jazz</b> (1970)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">70 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Inspiration_I_Feel">
              The Inspiration I Feel
            </a>
              <cite>The Inspiration I Feel The Inspiration I Feel is an album by flautist Herbie Mann featuring tunes associated with Ray Charles recorded in 1968 and released on the Atlantic label. The Inspiration I Feel Studio album by Herbie Mann Released 1968 Recorded May 6 &amp; 7, 1968 New York City Genre <b>Jazz</b> Length 34:28 Label Atlantic SD 1513 Producer Nesuhi Ertegun, Joel Dorn Herbie Mann chronology Windows Opened (1968) The Inspiration I Feel (1968) Memphis Underground (1968)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">78 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Milt_Turner">
              Milt Turner
            </a>
              <cite>...Turner After graduating from Pearl High School, he attended Tennessee State University, where he coincided with Hank Crawford, who he later recommended to join him in Ray Charles' band when he took over from William Peeples in the late 1950s. Milton Turner (1930-1993) was a <b>jazz</b> drummer. In 1962, he was a member of Phineas Newborn's trio with Leroy Vinnegar, on whose solo albums he would later appear, and in the early 1960s, Turner also recorded with Teddy Edwards. He never recorded as a leader....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">87 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Rare_Genius">
              Rare Genius
            </a>
              <cite>...studio recordings and demos made in the 1970s, 1980s and 1990s together with some contemporary instrumental and backing vocal parts. Rare Genius: The Undiscovered Masters Remix album by Ray Charles Released 2010 Genre Soul Length 41:36 Label Concord Producer Ray Charles, John Burk Ray Charles chronology Ray Sings, Basie Swings (2006) Rare Genius: The Undiscovered Masters (2010) Professional ratings Review scores Source Rating Allmusic (link) PopMatters (link) All About <b>Jazz</b> (link) favorable...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">91 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Tangerine_Records_(1962)">
              Tangerine Records (1962)
            </a>
              <cite>...in 1962. ABC-Paramount Records promoted and distributed it. Early singles labels were orange and later became black, red and white. Many of the later recordings are now sought after in "Northern Soul" circles. In 1973 Charles left ABC, closed Tangerine and started Crossover Records. Ray Charles Enterprises owns the catalog. Tangerine Records Parent company ABC-Paramount Records Founded 1962 Founder Ray Charles Defunct 1973 Distributor(s) ABC-Paramount Records Genre R&amp;B, soul music, <b>jazz</b> music...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">87 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Sings,_Basie_Swings">
              Ray Sings, Basie Swings
            </a>
              <cite>...from 1973 with newly recorded instrumental tracks by the contemporary Count Basie Orchestra. Professional ratings Review scores Source Rating AllMusic Ray Sings, Basie Swings Compilation album by Ray Charles, Count Basie Orchestra Released October 3, 2006 (2006-10-03) Recorded Mid-1970s, February - May 2006 Studio Los Angeles Genre Soul, <b>jazz</b>, Swing Label Concord/Hear Music Producer Gregg Field Ray Charles chronology Genius &amp; Friends (2005) Ray Sings, Basie Swings (2006) Rare Genius: The Undiscovered Masters (2010)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">91 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/I_Remember_Brother_Ray">
              I Remember Brother Ray
            </a>
              <cite>...is an album by saxophonist David "Fathead" Newman, paying tribute to his bandleader and mentor Ray Charles, which was recorded in 2004 and released on the HighNote label the following year. I Remember Brother Ray Studio album by David "Fathead" Newman Released January 11, 2005 Recorded August 14, 2004 Studio Van Gelder Studio, Englewood Cliffs, NJ Genre <b>Jazz</b> Length 50:39 Label HighNote HCD 7135 Producer David "Fathead" Newman, Houston Person David "Fathead" Newman chronology Song for the New Man (2004) I Remember Brother Ray (2005) Cityscape (2006)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">96 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Light_Out_of_Darkness_(A_Tribute_to_Ray_Charles)">
              Light Out of Darkness (A Tribute to Ray Charles)
            </a>
              <cite>...to Ray Charles) is a 1993 studio album by Shirley Horn, recorded in tribute to Ray Charles. Light Out of Darkness (A Tribute to Ray Charles) Studio album by Shirley Horn Released 1993 Recorded April 30 and May 1–3, 1993, Clinton Recording Studios, New York City Genre Vocal <b>jazz</b> Length 62:53 Label Verve Producer Shirley Horn, Sheila Mathis, Richard Seidel, Lynn Butterer Shirley Horn chronology Here's to Life (1992) Light Out of Darkness (A Tribute to Ray Charles) (1993) I Love You, Paris (1994)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">100 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Soul_Meeting">
              Soul Meeting
            </a>
              <cite>...in 1957 and released in 1961 on Atlantic Records. The album was later re-issued together with the other Charles–Jackson recording, Soul Brothers, on a 2 CD compilation together with other 'bonus' tracks from the same recording sessions. Professional ratings Review scores Source Rating Down Beat (Original Lp release) AllMusic link Soul Meeting Studio album by Ray Charles, Milt Jackson Released 1961 Recorded April 10, 1958 Genre R&amp;B, <b>jazz</b> Length 37:43 Label Atlantic Producer Tom Dowd Ray Charles chronology The Genius Sings the Blues (1961) Soul Meeting (1961) The Genius After Hours (1961) Alternative cover compilation CD re-issue...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">114 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Charles_in_Concert">
              Ray Charles in Concert
            </a>
              <cite>...between 1958 and 1975. In Concert Compilation album by Ray Charles Released 2003 Recorded Newport <b>Jazz</b> Festival (1958 July 5), Herndon Stadium Atlanta (1959 May 19), Sportpalast Berlin (1962 March 6), Shrine Auditorium Los Angeles (1964 September 20), Tokyo (1975 November 27) and Yokohama (1975 November 30) Genre R&amp;B, soul Length 2 hours Label Rhino Handmade Producer Nesuhi Ertegun (Newport), Zenas Sears (Atlanta), Norman Granz (Berlin), Sid Feller (Los Angeles) and Ray Charles (Tokyo / Yokohama)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">118 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Sticks_and_Stones_(Titus_Turner_song)">
              Sticks and Stones (Titus Turner song)
            </a>
              <cite>...in a 1960 version by Ray Charles, who added the Latin drum part. It was his first R&amp;B hit with ABC-Paramount, followed in 1961 with "Hit The Road Jack". The song was also covered by Jerry Lee Lewis, The Zombies, Wanda Jackson and The Kingsmen, as well as Joe Cocker on Mad Dogs and Englishmen, and Elvis Costello in 1994 on the extended play version of Kojak Variety. In 1997, <b>jazz</b> singer Roseanna Vitro included the tune in her tribute to Charles, Catchin’ Some Rays: The Music of Ray Charles....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">113 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Do_the_Twist!_with_Ray_Charles">
              Do the Twist! with Ray Charles
            </a>
              <cite>...peaked at #11. Do the Twist! with Ray Charles Greatest hits album by Ray Charles Released 1961 Recorded 1954-1960 Genre R&amp;B, Soul, <b>Jazz</b> Length 32:39 Label Atlantic Ray Charles chronology The Genius Sings the Blues (1961) Do the Twist! with Ray Charles (1961) Soul Meeting (1961) Professional ratings Review scores Source Rating Allmusic (link) The Rolling Stone Record Guide In 1963, the album got a new cover and was renamed The Greatest Ray Charles. Track listing and catalog number (Atlantic 8054) remained the same....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">120 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Great_Ray_Charles">
              The Great Ray Charles
            </a>
              <cite>...<b>jazz</b> album. Later CD re-issues often include as a bonus, six of eight tracks from The Genius After Hours. The original cover was by Marvin Israel. Professional ratings Review scores Source Rating Allmusic The Great Ray Charles Studio album by Ray Charles Released August 1957 Recorded April 30 - November 26, 1956 in New York City Genre Bebop Length 37:37 Label Atlantic Producer Ahmet Ertegün, Jerry Wexler Ray Charles chronology Ray Charles (or, Hallelujah I Love Her So) (1957) The Great Ray......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">127 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Charles_Live">
              Ray Charles Live
            </a>
              <cite>...<b>Jazz</b> Festival in 1958 and at Herndon Stadium in Atlanta in 1959, respectively). Later CD re-issues of this compilation include an additional, previously unreleased, track from the 1958 Newport concert, "Swanee River Rock." Live Live album by Ray Charles Released 1973 Recorded July 5, 1958 / May 28, 1959 Genre Soul, R&amp;B Length 71:55 Label Atlantic 503 Producer Nesuhi Ertegün / Zenas Sears Ray Charles chronology From the Pages of My Mind (1986) Live (1973) Just Between Us (1988) Professional......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">133 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Blue_Funk_(Ray_Charles_song)">
              Soul Brothers
            </a>
              <cite>...on the original LP releases. Soul Brothers Studio album by Ray Charles, Milt Jackson Released June 1958 Recorded September 12, 1957 (Tracks 1-2) and April 10, 1958 (Tracks 3-7), in New York City Genre R&amp;B, <b>jazz</b> Length 38:42 Label Atlantic, Studio One Producer Nesuhi Ertegun Ray Charles chronology Yes Indeed! (1958) Soul Brothers (1958) What'd I Say (1959) alternate release cover compilation CD / re-issue Professional ratings Review scores Source Rating AllMusic Down Beat (Original Lp release)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">135 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Soul_Brothers">
              Soul Brothers
            </a>
              <cite>...and the eleventh studio album by Milt Jackson and released by Atlantic Records in 1958. The album was later re-issued in a 2 CD compilation together with the other Charles–Jackson album Soul Meeting and included additional tracks from the same recording sessions not present on the original LP releases. Soul Brothers Studio album by Ray Charles, Milt Jackson Released June 1958 Recorded September 12, 1957 (Tracks 1-2) and April 10, 1958 (Tracks 3-7), in New York City Genre R&amp;B, <b>jazz</b> Length 38:42 Label Atlantic, Studio One Producer Nesuhi Ertegun Ray Charles chronology Yes Indeed! (1958) Soul Brothers (1958) What'd I Say (1959) alternate release cover compilation CD / re-issue Professional ratings Review scores Source Rating AllMusic Down Beat (Original Lp release)...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">135 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Charles_and_Betty_Carter">
              Ray Charles and Betty Carter
            </a>
              <cite>...Betty Carter Studio album by Ray Charles and Betty Carter Released August 1961 Recorded August 23, 1960 - June 14, 1961 Genre <b>Jazz</b> Length 41:38 Label ABC Producer Sid Feller Ray Charles chronology Dedicated to You (1961) Ray Charles and Betty Carter (1961) The Genius Sings the Blues (1961) Betty Carter chronology The Modern Sound of Betty Carter (1960) Ray Charles and Betty Carter (1961) 'Round Midnight (1962) Alternative cover / re-issue 1998 Rhino CD re-issue with Dedicated to You Professional ratings Review scores Source Rating Allmusic...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">158 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ingredients_in_a_Recipe_for_Soul">
              Ingredients in a Recipe for Soul
            </a>
              <cite>...6, 1960–April 28, 1963 Genre R&amp;B, soul, country soul, vocal <b>jazz</b> Label ABC 465 Producer Sid Feller Ray Charles chronology Modern Sounds in Country and Western Music, Vol. 2 (1962) Ingredients in a Recipe for Soul (1963) Sweet &amp; Sour Tears (1964) Alternative cover 1997 Rhino CD re-issue with Have a Smile with Me In 1990, the album was released on compact disc by DCC with four bonus tracks. In 1997, it was packaged together with 1964's Have a Smile with Me on a two-for-one CD reissue on Rhino with historical liner notes. Professional ratings Review scores......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">162 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Genius_Sings_the_Blues">
              The Genius Sings the Blues
            </a>
              <cite>...<b>jazz</b>, and southern R&amp;B. The photo for the album cover was taken by renowned photographer Lee Friedlander. The Genius Sings the Blues was reissued in 2003 by Rhino Entertainment with liner notes by Billy Taylor. The Genius Sings the Blues Compilation album by Ray Charles Released October 1961 Recorded 1952–1960 Genre Rhythm and blues, piano blues, soul Length 34:19 Label Atlantic SD-8052 Producer Ahmet Ertegün, Jerry Wexler Ray Charles chronology Ray Charles and Betty Carter (1961) The Genius......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">162 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Genius_of_Ray_Charles">
              The Genius of Ray Charles
            </a>
              <cite>...the sixth studio album by American recording artist Ray Charles, released in 1959 by Atlantic Records. The album eschewed the soul sound of his 1950s recordings, which fused <b>jazz</b>, gospel, and blues, for swinging pop with big band arrangements. It comprises a first half of big band songs and a second half of string-backed ballads. The Genius of Ray Charles sold fewer than 500,000 copies and charted at number 17 on the Billboard 200. "Let the Good Times Roll" and "Don't Let the Sun Catch You Cryin'" were released as singles in 1959. The Genius of Ray Charles Studio album by Ray Charles Released October 1959 Recorded May 6 and June 23, 1959 at 6 West Recording in New......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">172 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Charles_in_Person">
              Ray Charles in Person
            </a>
              <cite>...night in Atlanta, Georgia at Morris Brown College's Herndon Stadium. All tracks from this album together with those from Ray Charles at Newport were also released on the 1987 Atlantic compilation CD, Ray Charles Live. Ray Charles: In Person Live album by Ray Charles Released July 1960 Recorded May 28, 1959 Genre R&amp;B Length 29:19 Label Atlantic Producer Harris Zenas Ray Charles chronology The Genius of Ray Charles (1959) Ray Charles: In Person (1960) Genius + Soul = <b>Jazz</b> (1961) Re-issue cover 1987 re-issue / compilation Professional ratings Review scores Source Rating Allmusic The album was recorded by the concert sponsor, radio station WAOK. The station's lead disk jockey, Zenas "Daddy" Sears, recorded the album from the audience using a single microphone. The album is noted for its technical excellence in balancing band, singer, and audience, and also for its documentation of the jazzy R&amp;B Ray Charles sound prior to his great crossover success....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">176 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Don't_Let_the_Sun_Catch_You_Cryin'">
              Don&apos;t Let the Sun Catch You Cryin&apos;
            </a>
              <cite>...R&amp;B Sides" and No. 95 on the Billboard Hot 100. It was also recorded by Jackie DeShannon on her 1965 album This is Jackie De Shannon, Paul McCartney on his 1990 live album Tripping the Live Fantastic, Jex Saarelaht and Kate Ceberano on their album Open the Door - Live at Mietta's (1992) and <b>jazz</b> singer Roseanna Vitro on her 1997 album Catchin’ Some Rays: The Music of Ray Charles. Karin Krog and Steve Kuhn include it on their 2005 album, Together Again. Steve Alaimo released a version in 1963...</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">185 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/I_Don't_Need_No_Doctor">
              I Don&apos;t Need No Doctor
            </a>
              <cite>...<b>jazz</b> guitar player John Scofield recorded a version for his album That's What I Say: John Scofield Plays the Music of Ray Charles in 2005, featuring the blues guitarist John Mayer on additional guitar and vocals. Mayer covered the song again with his band during his tour in summer 2007. A recorded live version from a Los Angeles show during that tour is available on Mayer's CD/DVD release Where the Light Is. A Ray Charles tribute album also provided the impetus for <b>jazz</b> singer Roseanna Vitro's......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">558 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/If_You_Go_Away">
              If You Go Away
            </a>
              <cite>...<b>Jazz</b> Length 3:49 Label Epic Records Songwriter(s) Jacques Brel, Rod McKuen Producer(s) Bob Morgan Damita Jo singles chronology "Gotta Travel On" (1965) "If You Go Away" (1966) "Walk Away" (1967) Damita Jo reached #10 on the Adult Contemporary chart and #68 on the Billboard Hot 100 in 1966 for her version of the song. Terry Jacks recorded a version of the song which was released as a single in 1974 and reached #29 on the Adult Contemporary chart, #68 on the Billboard Hot 100, and went to #8 in......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">204 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Anthology_(Ray_Charles_album)">
              Anthology (Ray Charles album)
            </a>
              <cite>...Charles' '60s and '70s ABC-Paramount material", while Rhino Records, the issuing label, refers to it in the liner notes as "the compact disc edition of Ray Charles' Greatest Hits", alluding to the two Rhino LPs issued the same year. It is one of the first CDs to be released by Rhino. Anthology Greatest hits album by Ray Charles Released 1988 Recorded 1960-1972 Genre R&amp;B soul <b>jazz</b> piano blues Length 67:25 (original), 66:18 (re-release) Label Rhino Producer Ray Charles Steve Hoffman Richard Foos Ray Charles chronology Just Between Us (1988) Anthology (1988) Would You Believe? (1990) Posthumous cover Professional ratings Review scores Source Rating AllMusic Charles, who retained the master rights (currently controlled by his estate since his June 2004 passing) to his ABC-Paramount recordings, supervised a remixing of the 20 songs on this compilation especially for this......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">265 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Ray_Charles">
              Ray Charles
            </a>
              <cite>...1960s Background information Birth name Ray Charles Robinson Born (1930-09-23)September 23, 1930 Albany, Georgia, U.S. Died June 10, 2004(2004-06-10) (aged 73) Beverly Hills, California, U.S. Genres R&amp;B soul blues gospel country <b>jazz</b> rock and roll Occupation(s) musician singer songwriter composer Instruments Vocals piano Years active 1947–2004 Labels Atlantic ABC Tangerine Warner Bros. Swing Time Concord Columbia Flashback Associated acts The Raelettes USA for Africa Billy Joel Gladys Knight Website raycharles.com Charles pioneered the soul music genre during the 1950s by combining blues, rhythm and blues, and gospel styles into the music he recorded for Atlantic. He contributed to the integration of country music......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">416 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/The_Pages_of_My_Mind">
              Ray Charles
            </a>
              <cite>...<b>jazz</b> rock and roll Occupation(s) musician singer songwriter composer Instruments Vocals piano Years active 1947–2004 Labels Atlantic ABC Tangerine Warner Bros. Swing Time Concord Columbia Flashback Associated acts The Raelettes USA for Africa Billy Joel Gladys Knight Website raycharles.com Charles pioneered the soul music genre during the 1950s by combining blues, rhythm and blues, and gospel styles into the music he recorded for Atlantic. He contributed to the integration of country music......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">416 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Here_We_Go_Again_(Ray_Charles_song)">
              Here We Go Again (Ray Charles song)
            </a>
              <cite>...was first covered in an instrumental <b>jazz</b> format, and many of the more recent covers have been sung as duets, such as one with Willie Nelson and Norah Jones with Wynton Marsalis accompanying. The song was released on their 2011 tribute album Here We Go Again: Celebrating the Genius of Ray Charles. The song lent its name to Red Steagall's 2007 album as well. Cover versions have appeared on compilation albums by a number of artists, even some who did not release "Here We Go Again" as a single....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">417 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Modern_Sounds_in_Country_and_Western_Music">
              Modern Sounds in Country and Western Music
            </a>
              <cite>...<b>jazz</b>. Charles produced the album with Sid Feller, who helped the singer select songs to record, and performed alongside saxophonist Hank Crawford, a string section conducted by Marty Paich, and a big band arranged by Gil Fuller and Gerald Wilson. Modern Sounds in Country and Western Music was an immediate critical and commercial success. The album and its four hit singles brought Charles greater mainstream notice and recognition in the pop market, as well as airplay on both R&amp;B and country radio......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">424 words</div>
)SEARCHRESULT"
};

// Snippets (i.e. the contents of the <cite> element) in the search results can
// slightly vary depending on
//
// - the version of libxapian (for example, in various Packages CI builds)
// - the parameters of the pagination (if using libzim before v7.2.2).
//
// In order to be able to share the same expected output data
// LARGE_SEARCH_RESULTS between multiple build platforms and test-points
// of the TaskbarlessServerTest.searchResults test-case
//
// 1. Snippets are excluded from the plain-text comparison of actual and
//    expected HTML strings. This is done with the help of the
//    function maskSnippetsInSearchResults()
//
// 2. Snippets are checked separately. If a plain-text comparison fails
//    then a weaker comparison is attempted. Currently it works by testing
//    that the actual snippet is a substring of the "expected" snippet
//    (the "..." omitted text markes on the snippet boundaries are taken
//    into account). The implementation of that approach is via the
//    isSubSnippet() function.
//
//    Therefore the "expected" snippets in the test data must be a union of
//    all possible snippets produced at runtime for a given (document, search
//    terms) pair on all platforms of interest:
//
//    - Overlapping snippets must be properly merged
//
//    - Non-overlapping snippets can be joined with a " ... " in between.
//

std::string maskSnippetsInSearchResults(std::string s)
{
  return replace(s, "<cite>.+</cite>", "<cite>SNIPPET TEXT WAS MASKED</cite>");
}

bool isValidSnippet(const std::string& s)
{
  return s.size() >= 250
      && s.find("<b>")  != std::string::npos
      && s.find("</b>") != std::string::npos;
}

size_t leadingDotCount(const std::string& s)
{
  return s.find_first_not_of(".");
}

size_t trailingDotCount(const std::string& s)
{
  return s.size() - 1 - s.find_last_not_of(".");
}

bool isSubSnippet(std::string subSnippet, const std::string& superSnippet)
{
  const auto leadingDotCountInSubSnippet = leadingDotCount(subSnippet);
  const auto trailingDotCountInSubSnippet = trailingDotCount(subSnippet);
  const bool subSnippetIsHeadless = leadingDotCountInSubSnippet >= 3;
  const bool subSnippetIsTailless = trailingDotCountInSubSnippet >= 3;
  if ( subSnippetIsHeadless )
  {
    subSnippet = subSnippet.substr(leadingDotCountInSubSnippet);
  }

  if ( subSnippetIsTailless )
  {
    subSnippet = subSnippet.substr(0, subSnippet.size() - trailingDotCountInSubSnippet);
  }

  const auto pos = superSnippet.find(subSnippet);
  if ( pos == std::string::npos )
    return false;

  if ( subSnippetIsHeadless == (pos == 0) )
    return false;

  if ( subSnippetIsTailless == (pos + subSnippet.size() == superSnippet.size()) )
    return false;

  return true;
}

#define  RAYCHARLESZIMID "6f1d19d0-633f-087b-fb55-7ac324ff9baf"
#define  EXAMPLEZIMID    "5dc0b3af-5df2-0925-f0ca-d2bf75e78af6"

TEST_F(TaskbarlessServerTest, searchResults)
{
  struct TestData
  {
    struct PaginationEntry
    {
      std::string label;
      size_t start;
      bool selected;
    };

    std::string query;
    int start;
    size_t resultsPerPage;
    size_t totalResultCount;
    size_t firstResultIndex;
    std::vector<std::string> results;
    std::vector<PaginationEntry> pagination;

    static std::string makeUrl(const std::string& query, int start, size_t resultsPerPage)
    {
      std::string url = "/ROOT/search?" + query;

      if ( start >= 0 ) {
        url += "&start=" + to_string(start);
      }

      if ( resultsPerPage != 0 ) {
        url += "&pageLength=" + to_string(resultsPerPage);
      }

      return url;
    }

    std::string getPattern() const
    {
      const std::string p = "pattern=";
      const size_t i = query.find(p);
      std::string r = query.substr(i + p.size());
      return r.substr(0, r.find("&"));
    }

    std::string url() const
    {
      return makeUrl(query, start, resultsPerPage);
    }

    std::string expectedHeader() const
    {
      if ( totalResultCount == 0 ) {
        return "\n        No results were found for <b>\"" + getPattern() + "\"</b>";
      }

      std::string header = R"(  Results
        <b>
          FIRSTRESULT-LASTRESULT
        </b> of <b>
          RESULTCOUNT
        </b> for <b>
          "PATTERN"
        </b>
      )";

      const size_t lastResultIndex = std::min(totalResultCount, firstResultIndex + results.size() - 1);
      header = replace(header, "FIRSTRESULT", to_string(firstResultIndex));
      header = replace(header, "LASTRESULT",  to_string(lastResultIndex));
      header = replace(header, "RESULTCOUNT", to_string(totalResultCount));
      header = replace(header, "PATTERN",     getPattern());
      return header;
    }

    std::string expectedResultsString() const
    {
      if ( results.empty() ) {
        return "\n        ";
      }

      std::string s;
      for ( const auto& r : results ) {
        s += "\n          <li>";
        s += maskSnippetsInSearchResults(r);
        s += "          </li>";
      }
      return s;
    }

    std::string expectedFooter() const
    {
      if ( pagination.empty() ) {
        return "\n      ";
      }

      std::ostringstream oss;
      oss << "\n        <ul>\n";
      for ( const auto& p : pagination ) {
        const auto url = makeUrl(query, p.start, resultsPerPage);
        oss << "            <li>\n";
        oss << "              <a ";
        if ( p.selected ) {
          oss << "class=\"selected\"";
        }
        oss << "\n                 href=\"" << url << "\">\n";
        oss << "                " << p.label << "\n";
        oss << "              </a>\n";
        oss << "            </li>\n";
      }
      oss << "        </ul>";
      return oss.str();
    }

    std::string expectedHtml() const
    {
      return makeSearchResultsHtml(
               getPattern(),
               expectedHeader(),
               expectedResultsString(),
               expectedFooter()
      );
    }

    TestContext testContext() const
    {
      return TestContext{ { "url", url() } };
    }

    void check(const std::string& html) const
    {
      EXPECT_EQ(maskSnippetsInSearchResults(html), expectedHtml())
        << testContext();

      checkSnippets(extractSearchResultSnippets(html));
    }

    typedef std::vector<std::string> Snippets;

    static Snippets extractSearchResultSnippets(const std::string& html)
    {
      Snippets snippets;
      const std::regex snippetRegex("<cite>(.*)</cite>");
      std::sregex_iterator snippetIt(html.begin(), html.end(), snippetRegex);
      const std::sregex_iterator end;
      for ( ; snippetIt != end; ++snippetIt)
      {
        const std::smatch snippetMatch = *snippetIt;
        snippets.push_back(snippetMatch[1].str());
      }
      return snippets;
    }

    void checkSnippets(const Snippets& snippets) const
    {
      ASSERT_EQ(snippets.size(), results.size());
      for ( size_t i = 0; i < results.size(); ++i )
      {
        const auto& r = results[i];
        const auto expectedSnippet = extractSearchResultSnippets(r);
        ASSERT_EQ(1u, expectedSnippet.size())
          << "Multiple snippets in test data:"
          << "\n" << r;

        if ( snippets[i] != expectedSnippet[0] ) {
          std::cout << "Trying a weaker check for a mismatching snippet...\n";
          checkMismatchingSnippet(snippets[i], expectedSnippet[0]);
        }
      }
    }

    void checkMismatchingSnippet(std::string actual, std::string expected) const
    {
      TestContext testContext{
                        { "url", url() },
                        { "actual snippet", actual },
                        { "expected snippet", expected }
      };

      ASSERT_TRUE(isValidSnippet(actual))   << testContext;
      ASSERT_TRUE(isValidSnippet(expected)) << testContext;

      if ( !isSubSnippet(actual, expected) ) {
        EXPECT_EQ(actual, expected) << testContext;
      }
    }
  };

  const TestData testData[] = {
    {
      /* query */          "pattern=velomanyunkan&books.id=" RAYCHARLESZIMID,
      /* start */            -1,
      /* resultsPerPage */   0,
      /* totalResultCount */ 0,
      /* firstResultIndex */ 0,
      /* results */          {},
      /* pagination */       {}
    },

    {
      /* query */          "pattern=razaf&books.id=" RAYCHARLESZIMID,
      /* start */            -1,
      /* resultsPerPage */   0,
      /* totalResultCount */ 1,
      /* firstResultIndex */ 1,
      /* results */ {
R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/We_Gonna_Move_to_the_Outskirts_of_Town">
              We Gonna Move to the Outskirts of Town
            </a>
              <cite>...to the Outskirts of Town "We Gonna Move to the Outskirts of Town" is a country blues song recorded September 3, 1936 by Casey Bill Weldon (voice and guitar). The song has been covered by many other musicians, most often under the title "I'm Gonna Move to the Outskirts of Town", and sometimes simply Outskirts of Town. All recordings seem to credit Weldon as songwriter, often as Weldon or as Will Weldon or as William Weldon. Some cover versions give credit also to Andy <b>Razaf</b> and/or to Roy Jacobs....</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">93 words</div>
)SEARCHRESULT"
      },
      /* pagination */ {}
    },

    {
      /* query */          "pattern=yellow&books.id=" RAYCHARLESZIMID,
      /* start */            -1,
      /* resultsPerPage */   0,
      /* totalResultCount */ 2,
      /* firstResultIndex */ 1,
      /* results */ {
R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/Eleanor_Rigby">
              Eleanor Rigby
            </a>
              <cite>...-side "<b>Yellow</b> Submarine" (double A-side) Released 5 August 1966 (1966-08-05) Format 7-inch single Recorded 28–29 April &amp; 6 June 1966 Studio EMI, London Genre Baroque pop, art rock Length 2:08 Label Parlophone (UK), Capitol (US) Songwriter(s) Lennon–McCartney Producer(s) George Martin The Beatles singles chronology "Paperback Writer" (1966) "Eleanor Rigby" / "<b>Yellow</b> Submarine" (1966) "Strawberry Fields Forever" / "Penny Lane" (1967) Music video "Eleanor Rigby" on YouTube The song continued the......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">201 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/If_You_Go_Away">
              If You Go Away
            </a>
              <cite>...standard and has been recorded by many artists, including Greta Keller, for whom some say McKuen wrote the lyrics. "If You Go Away" Single by Damita Jo from the album If You Go Away B-side "<b>Yellow</b> Days" Released 1966 Genre Jazz Length 3:49 Label Epic Records Songwriter(s) Jacques Brel, Rod McKuen Producer(s) Bob Morgan Damita Jo singles chronology "Gotta Travel On" (1965) "If You Go Away" (1966) "Walk Away" (1967) Damita Jo reached #10 on the Adult Contemporary chart and #68 on the Billboard Hot 100 in 1966 for her version of the song. Terry Jacks recorded a version of the song which was released as a single in 1974 and reached #29 on the Adult Contemporary chart, #68 on the......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">204 words</div>
)SEARCHRESULT"
      },
      /* pagination */ {}
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            -1,
      /* resultsPerPage */   100,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 1,
      /* results */ LARGE_SEARCH_RESULTS,
      /* pagination */ {}
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            -1,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 1,
      /* results */ {
        LARGE_SEARCH_RESULTS[0],
        LARGE_SEARCH_RESULTS[1],
        LARGE_SEARCH_RESULTS[2],
        LARGE_SEARCH_RESULTS[3],
        LARGE_SEARCH_RESULTS[4],
      },

      /* pagination */ {
        { "1", 0,  true  },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "▶", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            5,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 6,
      /* results */ {
        LARGE_SEARCH_RESULTS[5],
        LARGE_SEARCH_RESULTS[6],
        LARGE_SEARCH_RESULTS[7],
        LARGE_SEARCH_RESULTS[8],
        LARGE_SEARCH_RESULTS[9],
      },

      /* pagination */ {
        { "1", 0,  false },
        { "2", 5,  true  },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "6", 25, false },
        { "▶", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            10,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 11,
      /* results */ {
        LARGE_SEARCH_RESULTS[10],
        LARGE_SEARCH_RESULTS[11],
        LARGE_SEARCH_RESULTS[12],
        LARGE_SEARCH_RESULTS[13],
        LARGE_SEARCH_RESULTS[14],
      },

      /* pagination */ {
        { "1", 0,  false },
        { "2", 5,  false },
        { "3", 10, true  },
        { "4", 15, false },
        { "5", 20, false },
        { "6", 25, false },
        { "7", 30, false },
        { "▶", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            15,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 16,
      /* results */ {
        LARGE_SEARCH_RESULTS[15],
        LARGE_SEARCH_RESULTS[16],
        LARGE_SEARCH_RESULTS[17],
        LARGE_SEARCH_RESULTS[18],
        LARGE_SEARCH_RESULTS[19],
      },

      /* pagination */ {
        { "1", 0,  false },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, true  },
        { "5", 20, false },
        { "6", 25, false },
        { "7", 30, false },
        { "8", 35, false },
        { "▶", 40, false },
      }
    },

    {
      /* query */            "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            20,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 21,
      /* results */ {
        LARGE_SEARCH_RESULTS[20],
        LARGE_SEARCH_RESULTS[21],
        LARGE_SEARCH_RESULTS[22],
        LARGE_SEARCH_RESULTS[23],
        LARGE_SEARCH_RESULTS[24],
      },

      /* pagination */ {
        { "1", 0,  false },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, true  },
        { "6", 25, false },
        { "7", 30, false },
        { "8", 35, false },
        { "9", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            25,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 26,
      /* results */ {
        LARGE_SEARCH_RESULTS[25],
        LARGE_SEARCH_RESULTS[26],
        LARGE_SEARCH_RESULTS[27],
        LARGE_SEARCH_RESULTS[28],
        LARGE_SEARCH_RESULTS[29],
      },

      /* pagination */ {
        { "◀", 0,  false },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "6", 25, true  },
        { "7", 30, false },
        { "8", 35, false },
        { "9", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            30,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 31,
      /* results */ {
        LARGE_SEARCH_RESULTS[30],
        LARGE_SEARCH_RESULTS[31],
        LARGE_SEARCH_RESULTS[32],
        LARGE_SEARCH_RESULTS[33],
        LARGE_SEARCH_RESULTS[34],
      },

      /* pagination */ {
        { "◀", 0,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "6", 25, false },
        { "7", 30, true  },
        { "8", 35, false },
        { "9", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            35,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 36,
      /* results */ {
        LARGE_SEARCH_RESULTS[35],
        LARGE_SEARCH_RESULTS[36],
        LARGE_SEARCH_RESULTS[37],
        LARGE_SEARCH_RESULTS[38],
        LARGE_SEARCH_RESULTS[39],
      },

      /* pagination */ {
        { "◀", 0,  false },
        { "4", 15, false },
        { "5", 20, false },
        { "6", 25, false },
        { "7", 30, false },
        { "8", 35, true  },
        { "9", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            40,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 41,
      /* results */ {
        LARGE_SEARCH_RESULTS[40],
        LARGE_SEARCH_RESULTS[41],
        LARGE_SEARCH_RESULTS[42],
        LARGE_SEARCH_RESULTS[43],
      },

      /* pagination */ {
        { "◀", 0,  false },
        { "5", 20, false },
        { "6", 25, false },
        { "7", 30, false },
        { "8", 35, false },
        { "9", 40, true  },
      }
    },

    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            21,
      /* resultsPerPage */   3,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 22,
      /* results */ {
        LARGE_SEARCH_RESULTS[21],
        LARGE_SEARCH_RESULTS[22],
        LARGE_SEARCH_RESULTS[23],
      },

      /* pagination */ {
        {  "◀", 0,  false },
        {  "4", 9,  false },
        {  "5", 12, false },
        {  "6", 15, false },
        {  "7", 18, false },
        {  "8", 21, true  },
        {  "9", 24, false },
        { "10", 27, false },
        { "11", 30, false },
        { "12", 33, false },
        {  "▶", 42, false },
      }
    },

    // This test-point only documents how the current implementation
    // works, not how it should work!
    {
      /* query */          "pattern=jazz&books.id=" RAYCHARLESZIMID,
      /* start */            45,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 46,
      /* results */ {},

      /* pagination */ {
        { "◀", 0,  false },
        { "6", 25, false },
        { "7", 30, false },
        { "8", 35, false },
        { "9", 40, false  },
      }
    },

    // We must return results from the two books
    {
      /* query */          "pattern=travel"
                           "&books.id=" RAYCHARLESZIMID
                           "&books.id=" EXAMPLEZIMID,
      /* start */            0,
      /* resultsPerPage */   10,
      /* totalResultCount */ 2,
      /* firstResultIndex */ 1,
      /* results */          {
R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/If_You_Go_Away">
              If You Go Away
            </a>
              <cite>...<b>Travel</b> On" (1965) "If You Go Away" (1966) "Walk Away" (1967) Damita Jo reached #10 on the Adult Contemporary chart and #68 on the Billboard Hot 100 in 1966 for her version of the song. Terry Jacks recorded a version of the song which was released as a single in 1974 and reached #29 on the Adult Contemporary chart, #68 on the Billboard Hot 100, and went to #8 in the UK. The complex melody is partly derivative of classical music - the poignant "But if you stay..." passage comes from Franz Liszt's......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">204 words</div>
)SEARCHRESULT",

R"SEARCHRESULT(
            <a href="/ROOT/example/Wikibooks.html">
              Wikibooks
            </a>
              <cite>...<b>Travel</b> guide Wikidata Knowledge database Commons Media repository Meta Coordination MediaWiki MediaWiki software Phabricator MediaWiki bug tracker Wikimedia Labs MediaWiki development The Wikimedia Foundation is a non-profit organization that depends on your voluntarism and donations to operate. If you find Wikibooks or other projects hosted by the Wikimedia Foundation useful, please volunteer or make a donation. Your donations primarily helps to purchase server equipment, launch new projects......</cite>
              <div class="book-title">from Wikibooks</div>
              <div class="informations">538 words</div>
)SEARCHRESULT"
      },
      /* pagination */       {}
    },

    // Only RayCharles is in English.
    // [TODO] We should extend our test data to have another zim file in english returning results.
    {
          /* query */          "pattern=travel"
                               "&books.filter.lang=eng",
          /* start */            0,
          /* resultsPerPage */   10,
          /* totalResultCount */ 1,
          /* firstResultIndex */ 1,
          /* results */          {
R"SEARCHRESULT(
            <a href="/ROOT/zimfile/A/If_You_Go_Away">
              If You Go Away
            </a>
              <cite>...<b>Travel</b> On" (1965) "If You Go Away" (1966) "Walk Away" (1967) Damita Jo reached #10 on the Adult Contemporary chart and #68 on the Billboard Hot 100 in 1966 for her version of the song. Terry Jacks recorded a version of the song which was released as a single in 1974 and reached #29 on the Adult Contemporary chart, #68 on the Billboard Hot 100, and went to #8 in the UK. The complex melody is partly derivative of classical music - the poignant "But if you stay..." passage comes from Franz Liszt's......</cite>
              <div class="book-title">from Ray Charles</div>
              <div class="informations">204 words</div>
)SEARCHRESULT",
          },
          /* pagination */       {}
        },

    // Adding a book (without match) doesn't change the results
    {
      /* query */          "pattern=jazz"
                           "&books.id=" RAYCHARLESZIMID
                           "&books.id=" EXAMPLEZIMID,
      /* start */            -1,
      /* resultsPerPage */   100,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 1,
      /* results */ LARGE_SEARCH_RESULTS,
      /* pagination */ {}
    },

    {
      /* query */          "pattern=jazz"
                           "&books.filter.lang=eng",
      /* start */            -1,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 1,
      /* results */ {
        LARGE_SEARCH_RESULTS[0],
        LARGE_SEARCH_RESULTS[1],
        LARGE_SEARCH_RESULTS[2],
        LARGE_SEARCH_RESULTS[3],
        LARGE_SEARCH_RESULTS[4],
      },

      /* pagination */ {
        { "1", 0,  true  },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "▶", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz"
                           "&books.filter.tag=wikipedia",
      /* start */            -1,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 1,
      /* results */ {
        LARGE_SEARCH_RESULTS[0],
        LARGE_SEARCH_RESULTS[1],
        LARGE_SEARCH_RESULTS[2],
        LARGE_SEARCH_RESULTS[3],
        LARGE_SEARCH_RESULTS[4],
      },

      /* pagination */ {
        { "1", 0,  true  },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "▶", 40, false },
      }
    },

    {
      /* query */          "pattern=jazz"
                           "&books.filter.lang=eng"
                           "&books.filter.title=Ray%20Charles",
      /* start */            -1,
      /* resultsPerPage */   5,
      /* totalResultCount */ 44,
      /* firstResultIndex */ 1,
      /* results */ {
        LARGE_SEARCH_RESULTS[0],
        LARGE_SEARCH_RESULTS[1],
        LARGE_SEARCH_RESULTS[2],
        LARGE_SEARCH_RESULTS[3],
        LARGE_SEARCH_RESULTS[4],
      },

      /* pagination */ {
        { "1", 0,  true  },
        { "2", 5,  false },
        { "3", 10, false },
        { "4", 15, false },
        { "5", 20, false },
        { "▶", 40, false },
      }
    },
  };

  for ( const auto& t : testData ) {
    const auto r = zfs1_->GET(t.url().c_str());
    EXPECT_EQ(r->status, 200);
    t.check(r->body);
  }
}
