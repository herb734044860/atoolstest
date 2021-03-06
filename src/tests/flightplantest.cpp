/*****************************************************************************
* Copyright 2015-2019 Alexander Barthel alex@littlenavmap.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "flightplantest.h"

#include "geo/pos.h"
#include "geo/linestring.h"

using atools::fs::pln::Flightplan;
using atools::fs::pln::FlightplanIO;
using atools::fs::pln::FlightplanEntry;

const static QLatin1Literal OUTPUT("flightplan_output");

FlightplanTest::FlightplanTest()
{

}

void FlightplanTest::runtest(int argc, char *argv[])
{
  FlightplanTest tst;
  QTest::qExec(&tst, argc, argv);
}

void FlightplanTest::initTestCase()
{
  QDir().mkpath(OUTPUT);

  io.load(flightplan, ":/test/resources/flightplan.pln");

  QCOMPARE(flightplan.getEntries().size(), 18);

  io.load(flightplanUser, ":/test/resources/_test_flp_user.pln");

  QCOMPARE(flightplanUser.getEntries().size(), 12);
}

void FlightplanTest::cleanupTestCase()
{
  flightplan.clear();
  QCOMPARE(flightplan.getEntries().size(), 0);
}


#ifdef FPR_TEST
extern "C" {
#include "/home/alex/Downloads/Majestic/CFMCSystem_types.h"
}

void FlightplanTest::testReadFpr()
{
  QFile fprFile("/home/alex/ownCloud/Flight Simulator/Flightplans/EDDHLIRF.fpr");

  if(fprFile.open(QIODevice::ReadOnly))
  {
    QDataStream ds(&fprFile);

    tFMS_flight_plan plan;
    memset(&plan, 0, sizeof(plan));

    qDebug() << "read" << ds.readRawData(reinterpret_cast<char *>(&plan), sizeof(plan));

    qDebug() << "done fpr";
  }
}

#endif

void FlightplanTest::testDetectFormat()
{
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/RANDOM_BIN_1.tst"), atools::fs::pln::NONE);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/RANDOM_BIN_2.tst"), atools::fs::pln::NONE);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/RANDOM_BIN_3.tst"), atools::fs::pln::NONE);

  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/flightplan-procs.pln"), atools::fs::pln::PLN_FSX);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/flightplan-fs9.pln"), atools::fs::pln::PLN_FS9);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/LIDTLSZS_FSC.pln"), atools::fs::pln::PLN_FSC);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/KSEA_KFLL_sid_star_trans.xml"), atools::fs::pln::FLIGHTGEAR);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/EDDF-LIRF.fgfp"), atools::fs::pln::FLIGHTGEAR);
  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/KORD-KSEA.fgfp"), atools::fs::pln::FLIGHTGEAR);

  QCOMPARE(FlightplanIO::detectFormat(":/test/resources/add-on.xml"), atools::fs::pln::NONE);
}

void FlightplanTest::testSaveFprDirect()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/enna_eddm.pln");

  for(FlightplanEntry& e:fp.getEntries())
    e.setAirway(QString());

  io.saveFpr(fp, OUTPUT + QDir::separator() + "_test_fpr_direct.fpr");

  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "_test_fpr_direct.fpr").size(), 36089);
}

void FlightplanTest::testSaveFprAirway()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_fpr.pln");
  io.saveFpr(fp, OUTPUT + QDir::separator() + "_test_fpr_airway.fpr");

  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "_test_fpr_airway.fpr").size(), 36089);
}

void FlightplanTest::testLoadFs9()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/flightplan-fs9.pln");

  QCOMPARE(fp.getEntries().size(), 13);
}

void FlightplanTest::testLoadFsc()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/LIDTLSZS_FSC.pln");

  QCOMPARE(fp.getEntries().size(), 27);
}

void FlightplanTest::testLoadFms()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/test_flightplan.fms");

  QCOMPARE(fp.getEntries().size(), 18);
}

void FlightplanTest::testLoadFms2()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/FMMT-FJDG.fms");

  QCOMPARE(fp.getEntries().size(), 15);
}

void FlightplanTest::testSaveFlpDirect()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_direct.pln");
  io.saveFlp(fp, OUTPUT + QDir::separator() + "result_flp_direct.flp");

#ifdef Q_OS_WIN32
  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "result_flp_direct.flp").size(), 1055);
#else
  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "result_flp_direct.flp").size(), 1010);
#endif
}

void FlightplanTest::testSaveFlpAirway()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_airway.pln");
  io.saveFlp(fp, OUTPUT + QDir::separator() + "result_flp_airway.flp");

#ifdef Q_OS_WIN32
  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "result_flp_airway.flp").size(), 1204);
#else
  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "result_flp_airway.flp").size(), 1130);
#endif
}

void FlightplanTest::testSaveFlpMixed()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_mixed.pln");
  io.saveFlp(fp, OUTPUT + QDir::separator() + "result_flp_mixed.flp");

#ifdef Q_OS_WIN32
  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "result_flp_mixed.flp").size(), 996);
#else
  QCOMPARE(QFileInfo(OUTPUT + QDir::separator() + "result_flp_mixed.flp").size(), 943);
#endif
}

void FlightplanTest::testSaveFltplanDirect()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_direct.pln");
  io.saveFltplan(fp, OUTPUT + QDir::separator() + "result_fltplan_direct.fltplan");

  // QCOMPARE(QFileInfo("result_fltplan_direct.fltplan").size(), 1010);
}

void FlightplanTest::testSaveFltplanAirway()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_airway.pln");
  io.saveFltplan(fp, OUTPUT + QDir::separator() + "result_fltplan_airway.fltplan");

  // QCOMPARE(QFileInfo("result_fltplan_airway.fltplan").size(), 1130);
}

void FlightplanTest::testSaveFltplanMixed()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_mixed.pln");
  io.saveFltplan(fp, OUTPUT + QDir::separator() + "result_fltplan_mixed.fltplan");

  // QCOMPARE(QFileInfo("result_fltplan_mixed.fltplan").size(), 943);
}

void FlightplanTest::testSaveFlightGearDirect()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_direct.pln");
  io.saveFlightGear(fp, OUTPUT + QDir::separator() + "result_fltplan_direct_fg.xml");
}

void FlightplanTest::testSaveFlightGearAirway()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_airway.pln");
  io.saveFlightGear(fp, OUTPUT + QDir::separator() + "result_fltplan_airway_fg.xml");
}

void FlightplanTest::testSaveFlightGearMixed()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/_test_flp_mixed.pln");
  io.saveFlightGear(fp, OUTPUT + QDir::separator() + "result_fltplan_mixed_fg.xml");
}

void FlightplanTest::testSaveFlightGearSidStarTrans()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/KSEA_KFLL_sid_star_trans.pln");
  io.saveFlightGear(fp, OUTPUT + QDir::separator() + "result_fltplan_sid_star_trans_fg.xml");
}

void FlightplanTest::testLoadFlightGearSidStarTrans()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/KSEA_KFLL_sid_star_trans.xml");

  QCOMPARE(fp.getEntries().size(), 121);

  io.saveFlightGear(fp, OUTPUT + QDir::separator() + "KSEA_KFLL_sid_star_trans.xml");
}

void FlightplanTest::testLoadFlightGearRunway()
{
  Flightplan fp;

  io.load(fp, ":/test/resources/EDDF_LIRF_rw.xml");
  QCOMPARE(fp.getEntries().size(), 25);
}

void FlightplanTest::testLoadFlightGear()
{
  Flightplan fp;
  io.load(fp, ":/test/resources/EDDF_LIRF.xml");
  QCOMPARE(fp.getEntries().size(), 22);

  fp.clear();
  io.load(fp, ":/test/resources/KORD-KSEA.fgfp");
  QCOMPARE(fp.getEntries().size(), 21);

  fp.clear();
  io.load(fp, ":/test/resources/EDDF-LIRF.fgfp");
  QCOMPARE(fp.getEntries().size(), 19);
}

void FlightplanTest::testSave()
{
  io.save(flightplan, OUTPUT + QDir::separator() + "test_flightplan.pln", "1710", atools::fs::pln::SAVE_NO_OPTIONS);
}

void FlightplanTest::testSaveClean()
{
  io.save(flightplan, OUTPUT + QDir::separator() + "result_flightplan_clean.pln", "1710", atools::fs::pln::SAVE_CLEAN);
}

void FlightplanTest::testSaveRte()
{
  io.saveRte(flightplan, OUTPUT + QDir::separator() + "result_flightplan.rte");
}

void FlightplanTest::testSaveFlp()
{
  io.saveFlp(flightplan, OUTPUT + QDir::separator() + "result_flightplan.flp");
}

void FlightplanTest::testSaveFms()
{
  io.saveFms(flightplan, OUTPUT + QDir::separator() + "result_flightplan.fms", "1710", false);
}

void FlightplanTest::testSaveLeveld()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Eduardo Gomes (SBEG) to Val De Cans Intl (SBBE).pln");
  io.saveLeveldRte(plan, OUTPUT + QDir::separator() + "result_flightplan_leveld_SBEG_SBBE.rte");
}

void FlightplanTest::testSaveLeveld2()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Gran Canaria (GCLP) to General Juan N Alvarez Intl (MMAA).pln");
  io.saveLeveldRte(plan, OUTPUT + QDir::separator() + "result_flightplan_leveld_GCLP_MMAA.rte");
}

void FlightplanTest::testSaveFeelthere()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Rotterdam The Hague (EHRD) to Palma De Mallorca (LEPA).pln");
  io.saveFeelthereFpl(plan, OUTPUT + QDir::separator() + "result_flightplan_feelthere_EHRD_LEPA.fpl", 384);
}

void FlightplanTest::testSaveFeelthere2()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Naples Capodichino (LIRN) to Casablanca Mohammed V Intl (GMMN).pln");
  io.saveFeelthereFpl(plan, OUTPUT + QDir::separator() + "result_flightplan_feelthere_LIRN_GMMN.fms", 377);
}

void FlightplanTest::testSaveEfbr()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Hamburg - Fuhlsbuettel (EDDH) to Fiumicino (LIRF).pln");
  io.saveEfbr(plan,
              OUTPUT + QDir::separator() + "result_flightplan_efb_EDDH_LIRF.efbr",
              "IDEKO ABMAL Z990 HLZ M852 POVEL Z94 GALMA M736 LIZUM N503 VIC L12 LUMAV M726 GAVRA Z806 GIKIN L865 TAQ",
              "1703", "23", "16R");
}

void FlightplanTest::testSaveEfbr2()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Hamburg - Fuhlsbuettel (EDDH) to Muenchen Franz-Josef Strauss (EDDM).pln");
  io.saveEfbr(plan,
              OUTPUT + QDir::separator() + "result_flightplan_efb_EDDH_EDDM.efbr",
              "HLZ ERT HOD BAY ERL DKB NEU",
              "1703", "23", "16R");
}

void FlightplanTest::testSaveQwRte()
{
  Flightplan plan;
  io.load(plan, ":/test/resources/IFR Hamburg - Fuhlsbuettel (EDDH) to Fiumicino (LIRF).pln");
  io.saveQwRte(plan, OUTPUT + QDir::separator() + "result_flightplan_qw_EDDH_LIRF.rte");
}

void FlightplanTest::testSaveMaddogMdr()
{
  {
    Flightplan plan;
    io.load(plan, ":/test/resources/IFR Zurich (LSZH) to Dublin (EIDW).pln");
    io.saveMdr(plan, OUTPUT + QDir::separator() + "result_flightplan_maddog_LSZH_EIDW.mdr");
  }
  {
    Flightplan plan;
    io.load(plan, ":/test/resources/_test_flp_airway.pln");
    io.saveMdr(plan, OUTPUT + QDir::separator() + "result_flightplan_maddog_airway.mdr");
  }
  {
    Flightplan plan;
    io.load(plan, ":/test/resources/_test_flp_direct.pln");
    io.saveMdr(plan, OUTPUT + QDir::separator() + "result_flightplan_maddog_direct.mdr");
  }
  {
    Flightplan plan;
    io.load(plan, ":/test/resources/_test_flp_mixed.pln");
    io.saveMdr(plan, OUTPUT + QDir::separator() + "result_flightplan_maddog_mixed.mdr");
  }
  {
    Flightplan plan;
    io.load(plan, ":/test/resources/_test_flp_user.pln");
    io.saveMdr(plan, OUTPUT + QDir::separator() + "result_flightplan_maddog_user.mdr");
  }
}

void FlightplanTest::testSaveGarminGns()
{
  io.saveGarminGns(flightplan,
                   OUTPUT + QDir::separator() + "result_flightplan_gns.fpl", atools::fs::pln::SAVE_NO_OPTIONS);
  io.saveGarminGns(flightplanUser,
                   OUTPUT + QDir::separator() + "result_flightplan_usr_gns.fpl", atools::fs::pln::SAVE_NO_OPTIONS);

  io.saveGarminGns(flightplan, OUTPUT + QDir::separator() + "result_flightplan_gns_uwp.fpl",
                   atools::fs::pln::SAVE_GNS_USER_WAYPOINTS);
  io.saveGarminGns(flightplanUser, OUTPUT + QDir::separator() + "result_flightplan_usr_gns_uwp.fpl",
                   atools::fs::pln::SAVE_GNS_USER_WAYPOINTS);
}

void FlightplanTest::testSaveGpx()
{
  atools::geo::LineString track;

  track.append(atools::geo::Pos(-8.912841278937394, 52.60663708318238, 0.1));
  track.append(atools::geo::Pos(-8.862844205905699, 52.61221376715999, 0.2));
  track.append(atools::geo::Pos(-8.662602813702559, 52.63407550093609, 99.2));
  track.append(atools::geo::Pos(-8.402765170981132, 52.63008327695571, 10000.));
  track.append(atools::geo::Pos(-8.051495013488424, 52.66678238662439, 11110));
  track.append(atools::geo::Pos(-7.791341410380108, 52.66146052390298, 1111.2121210));
  track.append(atools::geo::Pos(-7.590108505223442, 52.68145353757851, 0));
  track.append(atools::geo::Pos(-7.18720374863533, 52.72068953286778, 0));
  track.append(atools::geo::Pos(-6.985279544531381, 52.73958550237592, 0));
  track.append(atools::geo::Pos(-6.732500222209509, 52.76263774609386, 0));
  track.append(atools::geo::Pos(-6.479632750528286, 52.78541082562952, 0));
  track.append(atools::geo::Pos(-6.276865722530852, 52.80297420333915, 0));
  track.append(atools::geo::Pos(-6.132516801580161, 52.84700371769064, 0));
  track.append(atools::geo::Pos(-5.894403110840573, 52.93085126233986, 0));
  track.append(atools::geo::Pos(-5.690426214570678, 52.947194255398, 0));
  track.append(atools::geo::Pos(-5.405982190787223, 53.06408723300809, 0));
  track.append(atools::geo::Pos(-5.222656559035221, 53.17260544928833, 0));
  track.append(atools::geo::Pos(-4.935089915514705, 53.28809650054057, 0));
  track.append(atools::geo::Pos(-4.80113998157991, 53.39209095226846, 0));
  track.append(atools::geo::Pos(-4.555823767157808, 53.47190579119767, 0));
  track.append(atools::geo::Pos(-4.270423009332013, 53.61645094191466, 0));
  track.append(atools::geo::Pos(-4.166087201263535, 53.62312074969561, 0));
  track.append(atools::geo::Pos(-3.884425953076333, 53.79765505224868, 0));
  track.append(atools::geo::Pos(-3.73945912397213, 53.86910554379104, 0));
  track.append(atools::geo::Pos(-3.593974375468135, 53.94038081773563, 0));
  track.append(atools::geo::Pos(-3.294163218527808, 54.05076791853898, 0));
  track.append(atools::geo::Pos(-3.046473252877224, 54.15801961794879, 0));
  track.append(atools::geo::Pos(-2.754029619996447, 54.32899837080367, 0));
  track.append(atools::geo::Pos(-2.448835712111017, 54.43750890894066, 0));
  track.append(atools::geo::Pos(-2.195800470528419, 54.54291826334662, 0));
  track.append(atools::geo::Pos(-1.892359867855327, 54.68118513362709, 0));
  track.append(atools::geo::Pos(-1.690518336893881, 54.78332347174996, 0));
  track.append(atools::geo::Pos(-1.432812628445609, 54.88713176962266, 0));
  track.append(atools::geo::Pos(-1.224080755792302, 54.95746059638702, 0));
  track.append(atools::geo::Pos(-0.9627979354446528, 55.06003580912068, 0));
  track.append(atools::geo::Pos(-0.7078904763374477, 55.22413359664792, 0));
  track.append(atools::geo::Pos(-0.3389805355902535, 55.39079719171274, 0));
  track.append(atools::geo::Pos(0.03361637479769906, 55.55641979916691, 0));
  track.append(atools::geo::Pos(0.4073509998221839, 55.75194740941132, 0));
  track.append(atools::geo::Pos(0.7379796509195197, 55.85201384900677, 0));
  track.append(atools::geo::Pos(1.065522335902295, 56.01346625611421, 0));
  track.append(atools::geo::Pos(1.453876149946145, 56.1748965166454, 0));
  track.append(atools::geo::Pos(1.731716995110804, 56.3338717756897, 0));
  track.append(atools::geo::Pos(1.902042166015112, 56.39783399814279, 0));
  track.append(atools::geo::Pos(2.244045417308942, 56.52525016709198, 0));
  track.append(atools::geo::Pos(3.050763412376868, 56.93355704549422, 0));
  track.append(atools::geo::Pos(3.524031376731082, 57.12020608214483, 0));
  track.append(atools::geo::Pos(4.124633534179482, 57.36698952164147, 0));
  track.append(atools::geo::Pos(4.675133568499456, 57.58074630891856, 0));
  track.append(atools::geo::Pos(5.173535834977535, 57.79336787628487, 0));
  track.append(atools::geo::Pos(5.681269037708801, 58.00438446417066, 0));
  track.append(atools::geo::Pos(6.068416811143624, 58.18509876683003, 0));
  track.append(atools::geo::Pos(6.32724973627387, 58.27375824370404, 0));
  track.append(atools::geo::Pos(6.592948401614967, 58.39330611811003, 0));
  track.append(atools::geo::Pos(6.989684251561687, 58.54054948240487, 0));
  track.append(atools::geo::Pos(7.255475242314522, 58.62772014826746, 0));
  track.append(atools::geo::Pos(7.660729737334462, 58.77333146669589, 0));
  track.append(atools::geo::Pos(8.005629815749748, 58.92007807114206, 0));
  track.append(atools::geo::Pos(8.279660078534462, 59.00557026591063, 0));
  track.append(atools::geo::Pos(8.623328875075405, 59.08822806882669, 0));
  track.append(atools::geo::Pos(8.978023930265772, 59.20161425399949, 0));
  track.append(atools::geo::Pos(9.329575671264211, 59.28279131668219, 0));
  track.append(atools::geo::Pos(9.69677739981466, 59.42616427740027, 0));
  track.append(atools::geo::Pos(10.12807569474038, 59.53420355557642, 0));
  track.append(atools::geo::Pos(10.50300223223913, 59.67588855774969, 0));
  track.append(atools::geo::Pos(10.79985291759063, 59.75717149016049, 0));
  track.append(atools::geo::Pos(11.2552073604072, 59.89392945661672, 0));
  track.append(atools::geo::Pos(11.72336661070573, 60.06102869962545, 0));
  track.append(atools::geo::Pos(12.02953475259541, 60.1401592504839, 0));
  track.append(atools::geo::Pos(12.34732023540856, 60.25030986169234, 0));
  track.append(atools::geo::Pos(12.83101400714702, 60.41452063128339, 0));
  track.append(atools::geo::Pos(13.13805509561112, 60.46009730211096, 0));
  track.append(atools::geo::Pos(13.47158443453761, 60.56839842988352, 0));
  track.append(atools::geo::Pos(13.96536833250672, 60.69805944926567, 0));
  track.append(atools::geo::Pos(14.2949590316671, 60.77319861874825, 0));
  track.append(atools::geo::Pos(14.71440045392788, 60.87415936748467, 0));
  track.append(atools::geo::Pos(15.2254113283459, 61.0002712645603, 0));
  track.append(atools::geo::Pos(15.83428933166988, 61.15082465852722, 0));
  track.append(atools::geo::Pos(16.24029741484372, 61.18460224898168, 0));
  track.append(atools::geo::Pos(16.60455160890264, 61.28690814726248, 0));
  track.append(atools::geo::Pos(17.12341088342881, 61.37554232615991, 0));
  track.append(atools::geo::Pos(17.74051571892953, 61.48735769593392, 0));
  track.append(atools::geo::Pos(18.31500487280984, 61.63585215426635, 0));
  track.append(atools::geo::Pos(18.66489083611455, 61.67043404183732, 0));
  track.append(atools::geo::Pos(19.17571123202728, 61.82349530819141, 0));
  track.append(atools::geo::Pos(19.51570154190333, 61.95909084400962, 0));
  track.append(atools::geo::Pos(19.83996693884588, 62.0625067664572, 0));
  track.append(atools::geo::Pos(20.00720431214803, 62.18144405359653, 0));
  track.append(atools::geo::Pos(20.44179176132402, 62.30773239471177, 0));
  track.append(atools::geo::Pos(20.5335027059148, 62.43462384471677, 0));
  track.append(atools::geo::Pos(20.95899374010899, 62.52836727020136, 0));
  track.append(atools::geo::Pos(21.15282772453778, 62.78226083059219, 0));
  track.append(atools::geo::Pos(21.46227445697069, 63.05960058382947, 0));
  track.append(atools::geo::Pos(21.87198654936395, 63.22420306945868, 0));
  track.append(atools::geo::Pos(22.06359863544403, 63.34248027796801, 0));
  track.append(atools::geo::Pos(22.28561554429433, 63.49250516408075, 0));
  track.append(atools::geo::Pos(22.59852454477242, 63.63342079113544, 0));
  track.append(atools::geo::Pos(22.74250197926566, 63.79249423199968, 0));
  track.append(atools::geo::Pos(23.08828468178615, 63.96467162384977, 0));
  track.append(atools::geo::Pos(23.35875933241051, 64.1462867358041, 0));
  track.append(atools::geo::Pos(23.60367355652876, 64.29603194982342, 0));
  track.append(atools::geo::Pos(23.82786623615623, 64.51911974701888, 0));
  track.append(atools::geo::Pos(23.86030128083475, 64.55100015988036, 0));
  track.append(atools::geo::Pos(24.09284348175117, 64.77439433336943, 0));
  track.append(atools::geo::Pos(24.3878008037588, 64.9561457990981, 0));
  track.append(atools::geo::Pos(24.7310746358597, 65.17016045825017, 0));
  track.append(atools::geo::Pos(24.76091651743168, 65.20177753019559, 0));
  track.append(atools::geo::Pos(25.02114287245591, 65.42598551321238, 0));
  track.append(atools::geo::Pos(25.20901136408574, 65.58613071329582, 0));
  track.append(atools::geo::Pos(25.4016104361722, 65.74645242514005, 0));
  track.append(atools::geo::Pos(25.4592089236757, 65.88486967267335, 0));
  track.append(atools::geo::Pos(25.45937953247709, 66.06564856316029, 0));
  track.append(atools::geo::Pos(25.46727676548766, 66.24705057426256, 0));
  track.append(atools::geo::Pos(25.39481477466763, 66.36410465244371, 0));
  track.append(atools::geo::Pos(25.4037915101323, 66.54578186305432, 0));
  track.append(atools::geo::Pos(25.31371910742141, 66.73753740398854, 0));
  track.append(atools::geo::Pos(25.42215626783848, 66.9095087040966, 0));
  track.append(atools::geo::Pos(25.6329843927416, 67.07163134045614, 0));
  track.append(atools::geo::Pos(25.64437747500959, 67.25391201962002, 0));
  track.append(atools::geo::Pos(25.71544726456321, 67.3938879773459, 0));
  track.append(atools::geo::Pos(25.71210864878289, 67.54643516194078, 0));
  track.append(atools::geo::Pos(25.90038122272161, 67.67781150373187, 0));
  track.append(atools::geo::Pos(26.03618486859126, 67.85231498231109, 0));
  track.append(atools::geo::Pos(26.18090456666544, 67.95097516704641, 0));
  track.append(atools::geo::Pos(26.42533764565832, 68.11543862911387, 0));
  track.append(atools::geo::Pos(26.62384682820603, 68.24703264012129, 0));
  track.append(atools::geo::Pos(26.82504806459202, 68.37864192527719, 0));
  track.append(atools::geo::Pos(27.0289698042235, 68.51025439575722, 0));
  track.append(atools::geo::Pos(27.39504340889707, 68.66392254132261, 0));
  track.append(atools::geo::Pos(27.71295071061289, 68.78430061645503, 0));
  track.append(atools::geo::Pos(28.47118575597634, 68.9358187790303, 0));
  track.append(atools::geo::Pos(28.91587838570846, 69.04359864239059, 0));
  track.append(atools::geo::Pos(29.20164999168561, 69.20702571615317, 0));
  track.append(atools::geo::Pos(29.60868541158515, 69.35865657541576, 0));
  track.append(atools::geo::Pos(29.90336391749667, 69.44466508670483, 0));

  io.saveGpx(flightplan, OUTPUT + QDir::separator() + "result_flightplan.gpx", track, QVector<quint32>(), 10000);
}
