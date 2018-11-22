#include "unit-tests.h"
#include "unit-tests_Settings.h"
#include <src/MIDI.h>
#include <test/mocks/test-mocks_SerialMock.h>

BEGIN_MIDI_NAMESPACE

END_MIDI_NAMESPACE

// -----------------------------------------------------------------------------

BEGIN_UNNAMED_NAMESPACE

using namespace testing;
USING_NAMESPACE_UNIT_TESTS

template<unsigned Size>
struct VariableSysExSettings : midi::DefaultSettings
{
    static const unsigned SysExMaxSize = Size;
    static const unsigned SendPartialSysEx = Size;
};

typedef test_mocks::SerialMock<256> SerialMock;
typedef VariableSysExSettings<128> Settings;
typedef midi::MidiInterface<SerialMock, Settings> MidiInterface;

MidiInterface* midi;

class MidiInputCallbacksPartialSysEx : public Test
{
public:
    MidiInputCallbacksPartialSysEx()
        : mMidi(mSerial)
    {
    }
    virtual ~MidiInputCallbacksPartialSysEx()
    {
    }

protected:
    virtual void SetUp()
    {
        midi = &mMidi;
    }

    virtual void TearDown()
    {
        midi = nullptr;
    }

protected:
    SerialMock      mSerial;
    MidiInterface   mMidi;
};

// --

unsigned totalSize = 0;

void handleSysEx(byte* inData, unsigned inSize)
{
    totalSize += inSize;
    // EXPECT_NE(midi, nullptr);
    // midi->sendSysEx(inSize, inData, true);
}

// TEST_F(MidiInputCallbacksPartialSysEx, sysEx)
// {
//     mMidi.setHandleSystemExclusive(handleSysEx);
//     mMidi.begin(MIDI_CHANNEL_OMNI);
//     mMidi.turnThruOff();

//     static const unsigned rxSize = 15;
//     static const byte rxData[rxSize] = {
//         0xf0, 'H','e','l','l','o',',',' ','W','o','r','l','d','!', 0xf7
//     };
//     mSerial.mRxBuffer.write(rxData, rxSize);

//     for (unsigned i = 0; i < rxSize - 1; ++i)
//     {
//         EXPECT_EQ(mMidi.read(), false);
//     }
//     EXPECT_EQ(mMidi.read(), true);
//     EXPECT_EQ(mMidi.getType(),              midi::SystemExclusive);
//     EXPECT_EQ(mMidi.getChannel(),           0);
//     EXPECT_EQ(mMidi.getSysExArrayLength(),  rxSize);

//     EXPECT_EQ(unsigned(mSerial.mTxBuffer.getLength()), rxSize);
//     const std::vector<byte> sysExData(mMidi.getSysExArray(),
//                                       mMidi.getSysExArray() + rxSize);
//     EXPECT_THAT(sysExData, ElementsAreArray(rxData));
// }

TEST_F(MidiInputCallbacksPartialSysEx, sysExLong)
{
    mMidi.setHandleSystemExclusive(handleSysEx);
    mMidi.begin(MIDI_CHANNEL_OMNI);
    mMidi.turnThruOff();

    static const unsigned rxSize = 210;
    static const byte rxData[rxSize] = {
        0xf0,
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
            'H','e','l','l','o',',',' ','W','o','r','l','d','!',
        0xf7
    };
    mSerial.mRxBuffer.write(rxData, rxSize);

    for (unsigned i = 0; i < rxSize - 1; ++i)
    {
        // EXPECT_EQ(mMidi.read(), false);
        mMidi.read();
    }
    // EXPECT_EQ(mMidi.read(), true);
    EXPECT_EQ(mMidi.getType(),              midi::SystemExclusive);
    EXPECT_EQ(mMidi.getChannel(),           0);

    EXPECT_EQ(rxSize, totalSize);
    // EXPECT_EQ(mMidi.getSysExArrayLength(),  rxSize);

    // EXPECT_EQ(unsigned(mSerial.mTxBuffer.getLength()), rxSize);
    // const std::vector<byte> sysExData(mMidi.getSysExArray(),
    //                                   mMidi.getSysExArray() + rxSize);
    // EXPECT_THAT(sysExData, ElementsAreArray(rxData));
}

END_UNNAMED_NAMESPACE
