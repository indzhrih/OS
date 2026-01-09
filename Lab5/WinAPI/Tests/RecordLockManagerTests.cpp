#include <boost/test/unit_test.hpp>
#include "../Headers/RecordLockManager.h"

BOOST_AUTO_TEST_SUITE(RecordLockManagerTests)

BOOST_AUTO_TEST_CASE(BeginReadOnUnlockedRecordReturnsTrue) {
    RecordLockManager manager;
    bool result = manager.beginRead(1);
    if (result) manager.endRead(1);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginWriteOnUnlockedRecordReturnsTrue) {
    RecordLockManager manager;
    bool result = manager.beginWrite(2);
    if (result) manager.endWrite(2);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginWriteWhileReadLockExistsReturnsFalseForSameId) {
    RecordLockManager manager;
    manager.beginRead(3);
    bool result = !manager.beginWrite(3);
    manager.endRead(3);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginWriteWhileReadLockExistsOnAnotherIdReturnsTrue) {
    RecordLockManager manager;
    manager.beginRead(4);
    bool result = manager.beginWrite(5);
    manager.endWrite(5);
    manager.endRead(4);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginReadWhileWriteLockExistsReturnsFalseForSameId) {
    RecordLockManager manager;
    manager.beginWrite(6);
    bool result = !manager.beginRead(6);
    manager.endWrite(6);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginReadWhileWriteLockExistsOnAnotherIdReturnsTrue) {
    RecordLockManager manager;
    manager.beginWrite(7);
    bool result = manager.beginRead(8);
    manager.endRead(8);
    manager.endWrite(7);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(HasWriterReflectsWriteStateForId) {
    RecordLockManager manager;
    bool before = manager.hasWriter(9);
    manager.beginWrite(9);
    bool during = manager.hasWriter(9);
    manager.endWrite(9);
    bool result = !before && during;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(HasWriterReturnsFalseForDifferentId) {
    RecordLockManager manager;
    manager.beginWrite(10);
    bool result = !manager.hasWriter(11);
    manager.endWrite(10);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(EndReadWithoutBeginReturnsFalse) {
    RecordLockManager manager;
    bool result = !manager.endRead(12);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(EndWriteWithoutBeginReturnsFalse) {
    RecordLockManager manager;
    bool result = !manager.endWrite(13);
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
