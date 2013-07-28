/***************************************************************************
 *   Copyright (C) 2010-2012 by Oleg Khudyakov                             *
 *   prcoder@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FIT_H
#define FIT_H

#include "GPX.h"
#include "Log.h"

#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;

#pragma pack(1)
struct FITHeader
{
    uint8_t headerSize;
    uint8_t protocolVersion;
    uint16_t profileVersion;
    uint32_t dataSize;
    uint8_t signature[4];
    uint16_t headerCRC;
};

struct RecordNormalHeader
{
    uint8_t localMessageType:4;
    uint8_t reserved:2;
    uint8_t messageType:1;
    uint8_t headerType:1;
};

struct RecordCompressedTimeStampHeader
{
    uint8_t timeOffset:5;
    uint8_t localMessageType:2;
    uint8_t headerType:1;
};

union RecordHeader
{
    RecordNormalHeader normalHeader;
    RecordCompressedTimeStampHeader ctsHeader;
};

struct RecordFixed
{
    uint8_t reserved;
    uint8_t arch;
    uint16_t globalNum;
    uint8_t fieldsNum;
};

struct RecordField
{
    uint8_t definitionNum;
    uint8_t size;
    uint8_t baseType;
};

struct BaseTypeBits
{
    uint8_t baseTypeNum:5;
    uint8_t reserved:2;
    uint8_t endianAbility:1;
};

union BaseType
{
    BaseTypeBits bits;
    uint8_t byte;
};

struct ZeroFileRecord
{
    uint16_t index;
    uint8_t fileDataType;
    uint8_t recordType;
    uint16_t identifier;
    uint8_t fileDataTypeFlags;
    struct
    {
        uint8_t reserved:2;
        uint8_t crypto:1;
        uint8_t append:1;
        uint8_t archive:1;
        uint8_t erase:1;
        uint8_t write:1;
        uint8_t read:1;
    } generalFileFlags;
    uint32_t fileSize;
    uint32_t timeStamp;
};

struct DirectoryHeader
{
    uint8_t version;
    uint8_t structureLength;
    uint8_t timeFormat;
    uint8_t reserved[5];
    uint32_t currentSystemTime;
    uint32_t directoryModifiedTime;
};

#pragma pack()

enum BaseTypes
{
    BT_Enum = 0,
    BT_Int8,
    BT_UInt8,
    BT_Int16,
    BT_Uint16,
    BT_Int32,
    BT_UInt32,
    BT_String,
    BT_Float32,
    BT_Float64,
    BT_Uint8z,
    BT_Uint16z,
    BT_Uint32z,
    BT_ByteArray
};

struct RecordDef
{
    RecordFixed rfx;
    vector<RecordField> rf;
};

enum MessageFieldTypes
{
    MessageFieldTypeUnknown = 0,
    MessageFieldTypeCoord,
    MessageFieldTypeAltitude,
    MessageFieldTypeTimestamp,
    MessageFieldTypeTime,
    MessageFieldTypeWeight,
    MessageFieldTypeSpeed,
    MessageFieldTypeOdometr,
    MessageFieldTypeFileType,
    MessageFieldTypeManufacturer,
    MessageFieldTypeProduct,
    MessageFieldTypeGender,
    MessageFieldTypeLanguage,
    MessageFieldTypeSport,
    MessageFieldTypeEvent,
    MessageFieldTypeEventType
};

enum Manufacturers
{
    ManufacturerGarmin = 1,
    ManufacturerGarminFR405ANTFS,
    ManufacturerZephyr,
    ManufacturerDayton,
    ManufacturerIDT,
    ManufacturerSRM,
    ManufacturerQuarq,
    ManufacturerIBike,
    ManufacturerSaris,
    ManufacturerSparkHK,
    ManufacturerTanita,
    ManufacturerEchowell,
    ManufacturerDynastreamOEM,
    ManufacturerNautilus,
    ManufacturerDynastream,
    ManufacturerTimex,
    ManufacturerMetriGear,
    ManufacturerXelic,
    ManufacturerBeurer,
    ManufacturerCardioSport,
    ManufacturerAandD,
    ManufacturerHMM,
    ManufacturerSuunto,
    ManufacturerThitaElektronik,
    ManufacturerGPulse,
    ManufacturerCleanMobile,
    ManufacturerPedalBrain,
    ManufacturerPeaksware,
    ManufacturerSaxonar,
    ManufacturerLemondFitness,
    ManufacturerDexcom,
    ManufacturerWahooFitness,
    ManufacturerOctaneFitness,
    ManufacturerArchinoetics,
    ManufacturerTheHurtBox,
    ManufacturerCitizenSystems,
    ManufacturerUnknown1,
    ManufacturerOsynce,
    ManufacturerHolux,
    ManufacturerConcept2,
    ManufacturerUnknown2,
    ManufacturerOneGiantLeap,
    ManufacturerAceSensor,
    ManufacturerBrimBrothers,
    ManufacturerXplova,
    ManufacturerPerceptionDigital,
    ManufacturerBF1Systems,
    ManufacturerPioneer,
    ManufacturerSpantec,
    ManufacturerMetalogics,
    Manufacturer4IIIIS
};

enum GarminProducts
{
    GarminHRM1          = 1,
    GarminAXH01         = 2,
    GarminAXB01         = 3,
    GarminAXB02         = 4,
    GarminHRM2SS        = 5,
    GarminDsiAlf02      = 6,
    GarminFR405         = 717,
    GarminFR50          = 782,
    GarminFR60          = 988,
    GarminDsiAlf01      = 1011,
    GarminFR310XT       = 1018,
    GarminEDGE500       = 1036,
    GarminFR110         = 1124,
    GarminEDGE800       = 1169,
    GarminChirp         = 1253,
    GarminEDGE200       = 1325,
    GarminFR910XT       = 1328,
    GarminALF04         = 1341,
    GarminFR610         = 1345,
    GarminFR70          = 1436,
    GarminFR310XT4T     = 1446,
    GarminAMX           = 1461,
    GarminSDM4          = 10007,
    GarminTraningCenter = 20119,
    GarminConnect       = 65534
};

class ZeroFileContent
{
public:
    vector<uint8_t> activityFiles;
    vector<uint8_t> waypointsFiles;
    vector<uint8_t> courseFiles;
};

class FIT
{
public:
    FIT();
    ~FIT();

    uint16_t CRC_byte(uint16_t crc, uint8_t byte);
    string getDataString(uint8_t *ptr, uint8_t size, uint8_t baseType, uint8_t messageType, uint8_t fieldNum);
    bool parse(vector<uint8_t> &fitData, GPX &gpx);
    bool parseZeroFile(vector<uint8_t> &data, ZeroFileContent &zeroFileContent);

private:
    map<uint8_t, string> messageTypeMap;
    map<uint8_t, map<uint8_t, string> > messageFieldNameMap;
    map<uint8_t, map<uint8_t, uint8_t> > messageFieldTypeMap;
    map<uint8_t, string> dataTypeMap;
    map<uint8_t, map<uint8_t, string> > enumMap;
    map<uint8_t, string> manufacturerMap;
    map<uint8_t, map<uint16_t, string> > productMap;
    int16_t manufacturer;
};

#endif
