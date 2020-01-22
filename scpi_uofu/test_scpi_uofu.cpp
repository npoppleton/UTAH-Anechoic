
/// @test_scpi_uofu.cpp
///
/// Unit-test suite for SCPI parse functions
///


#include <catch/catch.hpp>
#include <scpi.h>
#include <cstring>
#include <string.h>

#define SCPI_LOCAL

#define TEST_SCPI(x)        rc = test_adapter(x, &reply, &reply_len, &event)
#define REQUIRE_REPLY(x) REQUIRE(0 == strncmp(reinterpret_cast<char*>(reply), x, reply_len))

using namespace std;


static const size_t  k_buffer_size    = 128;
static uint8_t buffer[k_buffer_size];

// **********************************************************************************
/// Allows unittests to be run against local SCPI library or against TCP/IP to target
///
/// @param p_cmd        - c_str pointer to a SCPI command under test
/// @param reply        - pointer to ASCII reply
/// @param reply_len    - pointer to size_t reply_len
/// @return             - scpi_input response (1 for query, 2 for command)
///
static int test_adapter(const char * p_cmd, uint8_t ** reply, size_t * reply_len, uint32_t * event)
{

    strncpy(reinterpret_cast<char*>(buffer), p_cmd, k_buffer_size);

#ifdef SCPI_LOCAL

    //size_t rlen(0);

    int rc = scpi_input(  buffer,
                        strlen(p_cmd) + 1,
                        reply,
                        reply_len,
                        event);

    //*reply_len = rlen;

    return rc;

#else
    //Test against target using TCP/IP
#endif
}

//  ****************************************************************************
TEST_CASE("Top Menu Single", "")
{
  uint8_t * reply;
  uint32_t event;
  size_t reply_len;
  int rc;
  char * needle;

  static const char * OPC       = "*OPC";
  static const char * OPC_e1    = "*OPC1";
  static const char * OPC_e2    = "*OP";
  static const char * OPCQ      = "*OPC?";
  static const char * OPCQ_e1   = "*OPC1?";
  static const char * OPCQ_e2   = "*OP?";
  static const char * RST       = "*RST";
  static const char * RST_e1    = "*RST1";
  static const char * RST_e2    = "*RS";
  static const char * IDNQ      = "*IDN?";
  static const char * IDNQ_e1   = "*IDN1?";
  static const char * IDNQ_e2   = "*ID?";


  SECTION("*IDN? - Success")
  {
      // ***********************************************
      /// Handles a top-level SCPI string being input from TCP
      ///
      /// @param p_buf*[in]    - pointer to a Rx buffer
      /// @param len[in]       - length of the Rx buffer
      ///
      /// @returns            -  0 proceed to next sub-menu
      ///                     -  2 indicates finished processing, with a command type
      ///                     -  1 indicates finished processing, with a query type
      ///                     - < 0 for errors

      TEST_SCPI(IDNQ);
      REQUIRE(1 == rc);

      // The IDN string contains "University of Utah" in the middle some place
      needle = strstr(reinterpret_cast<char*>(reply), "University of Utah");

      REQUIRE(needle > reinterpret_cast<char*>(reply));
      REQUIRE(k_scpi_root_q_idn == event);

  }



  SECTION("*IDN? - Failure")
  {

      TEST_SCPI(IDNQ_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(IDNQ_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }

  SECTION("*RST - Success")
  {

      TEST_SCPI(RST);
      REQUIRE(2 == rc);
      REQUIRE_REPLY("OK_CMD");
      REQUIRE(k_scpi_root_rst == event);
  }

  SECTION("*RST - Failure")
  {

      TEST_SCPI(RST_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(RST_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }

  SECTION("*OPC - Success")
  {

      TEST_SCPI(OPC);
      REQUIRE(2 == rc);
      REQUIRE_REPLY("OK_CMD");
      REQUIRE(k_scpi_root_opc == event);
  }

  SECTION("*OPC - Failure")
  {

      TEST_SCPI(OPC_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(OPC_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }

  SECTION("*OPC? - Success")
  {

      TEST_SCPI(OPCQ);
      REQUIRE(1 == rc);
      REQUIRE_REPLY("OK_QUERY");
      REQUIRE(k_scpi_root_q_opc == event);
  }

  SECTION("*OPC? - Failure")
  {

      TEST_SCPI(OPCQ_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(OPCQ_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }


}

//  ****************************************************************************
TEST_CASE("Menu :INPut:", "")
{
  uint8_t * reply;
  uint32_t event;
  size_t reply_len;
  int rc;
  char * needle;

  static const char * inp       = ":INPut:";
  static const char * inps      = ":INP:";
  static const char * inp_e1    = ":INPut1:";
  static const char * inp_e2    = ":INPu:";
  static const char * inp_e3    = ":IN:";



  SECTION(":INPut: / :INP: - Partial Success (no submenu)")
  {

      TEST_SCPI(inp);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR_PARTIAL");
      REQUIRE(k_scpi_root_input == event);

      TEST_SCPI(inps);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR_PARTIAL");
      REQUIRE(k_scpi_root_input == event);

  }

  SECTION(":INPut: / :INP: - Failure")
  {

      TEST_SCPI(inp_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(inp_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(inp_e3);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }

}


//  ****************************************************************************
TEST_CASE("Menu :INITiate:", "")
{
  uint8_t * reply;
  uint32_t event;
  size_t reply_len;
  int rc;
  char * needle;

  static const char * init      = ":INITiate:";
  static const char * inits     = ":INIT:";
  static const char * init_e1   = ":INIT1:";
  static const char * init_e2   = ":INI:";


  SECTION(":INITiate: / :INIT: - Partial Success (no submenu)")
  {

      TEST_SCPI(init);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR_PARTIAL");
      REQUIRE(k_scpi_root_initiate == event);

      TEST_SCPI(inits);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR_PARTIAL");
      REQUIRE(k_scpi_root_initiate == event);

  }

  SECTION(":INITiate: / :INIT: - Failure")
  {

      TEST_SCPI(init_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(init_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }

}


TEST_CASE("Menu :SENSe:", "")
{
  uint8_t * reply;
  uint32_t event;
  size_t reply_len;
  int rc;
  char * needle;

  static const char * sense     = ":SENSe:";
  static const char * sens      = ":SENS:";
  static const char * sens_e1   = ":SENS1:";
  static const char * sens_e2   = ":SENSe1:";
  static const char * sens_e3   = ":SEN:";


  SECTION(":SENSe: / :SENS: - Partial Success (no submenu)")
  {

      TEST_SCPI(sense);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR_PARTIAL");
      REQUIRE(k_scpi_root_sense == event);

      TEST_SCPI(sens);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR_PARTIAL");
      REQUIRE(k_scpi_root_sense == event);

  }

  SECTION(":SENSe: / :SENS: - Failure")
  {

      TEST_SCPI(sens_e1);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(sens_e2);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

      TEST_SCPI(sens_e3);
      REQUIRE(0 > rc);
      REQUIRE_REPLY("ERROR");
      REQUIRE(k_scpi_root_none == event);

  }

}


//  ****************************************************************************
TEST_CASE("Menu :INPut:POSition:", "")
{
    uint8_t * reply;
    uint32_t event;
    size_t reply_len;
    int rc;
    char * needle;

    static const char * pos       = ":INP:POSition:";
    static const char * poss      = ":INP:POS:";
    static const char * pos_e1    = ":INP:POS1:";
    static const char * pos_e2    = ":INP:PO:";
    static const char * pos_e3    = ":INP:POSitio:";
    static const char * pos_e4    = ":INP:POSition1:";

    SECTION(":INPut:POSition: - Partial Success (no submenu)")
    {

        TEST_SCPI(pos);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position == event);

        TEST_SCPI(poss);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position == event);

    }

    SECTION(":INPut:POSition: - Failure")
    {

        TEST_SCPI(pos_e1);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input == event);

        TEST_SCPI(pos_e2);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input == event);

        TEST_SCPI(pos_e3);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input == event);

        TEST_SCPI(pos_e4);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input == event);


    }


}


//  ****************************************************************************
TEST_CASE("Menu :INPut:POSition:a0[|a1|a2|a3]", "")
{
    uint8_t * reply;
    uint32_t event;
    size_t reply_len;
    int rc;
    char * needle;

    static const char * a0        = ":INP:POS:a0";
    static const char * a1        = ":INP:POS:a1";
    static const char * a2        = ":INP:POS:a2";
    static const char * a3        = ":INP:POS:a3";
    static const char * a0_e1     = ":INP:POS:a";
    static const char * a0_e2     = ":INP:POS:a01";

    SECTION(":INPut:POSition:a0[|a1|a2|a3] - Partial Success (no submenu)")
    {

        TEST_SCPI(a0);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0 == event);

        TEST_SCPI(a1);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a1 == event);

        TEST_SCPI(a2);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a2 == event);

        TEST_SCPI(a3);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a3 == event);

    }

    SECTION(":INPut:POSition:a0 - Failure (a0 menu item)")
    {


        TEST_SCPI(a0_e1);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position == event);

        TEST_SCPI(a0_e2);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position == event);


    }


}

//  ****************************************************************************
TEST_CASE("Menu :INPut:POSition:a0:ANGLe", "")
{
    uint8_t * reply;
    uint32_t event;
    size_t reply_len;
    int rc;
    char * needle;

    static const char * angle     = ":INP:POS:a0:ANGLe";
    static const char * angl      = ":INP:POS:a0:ANGL";
    static const char * a1        = ":INP:POS:a1:ANGL";
    static const char * a2        = ":INP:POS:a2:ANGL";
    static const char * a3        = ":INP:POS:a3:ANGL";
    static const char * a0_e1     = ":INP:POS:a0:ANG";
    static const char * a0_e2     = ":INP:POS:a0:ANGLe1";
    static const char * a0_e3     = ":INP:POS:a0:ANG1";

    SECTION(":INPut:POSition:a0:ANGLe - Partial Success (no submenu)")
    {

        TEST_SCPI(angle);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0_angle == event);

        TEST_SCPI(angl);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0_angle == event);

        TEST_SCPI(a1);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a1_angle == event);

        TEST_SCPI(a2);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a2_angle == event);

        TEST_SCPI(a3);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a3_angle == event);

    }

    SECTION(":INPut:POSition:a0:ANGLe - Failure (@ angle menu item)")
    {


        TEST_SCPI(a0_e1);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0 == event);

        TEST_SCPI(a0_e2);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0 == event);

        TEST_SCPI(a0_e3);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0 == event);


    }



}


//  ****************************************************************************
TEST_CASE("Menu :INPut:POSition:a0:ANGLe:IMMediate", "")
{
    uint8_t * reply;
    uint32_t event;
    size_t reply_len;
    int rc;
    char * needle;

    static const char * immediate       = ":INP:POS:a0:ANGLe:IMMediate";
    static const char * imm             = ":INP:POS:a0:ANGL:IMM";
    static const char * a1              = ":INP:POS:a1:ANGL:IMM";
    static const char * a2              = ":INP:POS:a2:ANGL:IMM";
    static const char * a3              = ":INP:POS:a3:ANGL:IMM";
    static const char * a0_e1           = ":INP:POS:a0:ANGL:IM";
    static const char * a0_e2           = ":INP:POS:a0:ANGL:IM1";
    static const char * a0_e3           = ":INP:POS:a0:ANGL:IMMediate1";

    SECTION(":INPut:POSition:a0:ANGLe:IMMediate - Success")
    {


        TEST_SCPI(immediate);
        REQUIRE(2 == rc);
        REQUIRE_REPLY("OK_CMD");
        REQUIRE(k_scpi_input_position_a0_immediate == event);

        TEST_SCPI(imm);
        REQUIRE(2 == rc);
        REQUIRE_REPLY("OK_CMD");
        REQUIRE(k_scpi_input_position_a0_immediate == event);

        TEST_SCPI(a1);
        REQUIRE(2 == rc);
        REQUIRE_REPLY("OK_CMD");
        REQUIRE(k_scpi_input_position_a1_immediate == event);

        TEST_SCPI(a2);
        REQUIRE(2 == rc);
        REQUIRE_REPLY("OK_CMD");
        REQUIRE(k_scpi_input_position_a2_immediate == event);

        TEST_SCPI(a3);
        REQUIRE(2 == rc);
        REQUIRE_REPLY("OK_CMD");
        REQUIRE(k_scpi_input_position_a3_immediate == event);

    }

    SECTION(":INPut:POSition:a0:ANGLe:IMMediate - Failures")
    {


        TEST_SCPI(a0_e1);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0_angle == event);

        TEST_SCPI(a0_e2);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0_angle == event);

        TEST_SCPI(a0_e3);
        REQUIRE(0 > rc);
        REQUIRE_REPLY("ERROR_PARTIAL");
        REQUIRE(k_scpi_input_position_a0_angle == event);


    }



}



