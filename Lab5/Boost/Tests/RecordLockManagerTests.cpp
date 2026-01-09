#include <boost/test/unit_test.hpp>
#include "../Headers/RecordLockManager.h"

BOOST_AUTO_TEST_SUITE(RecordLockManagerTests)

BOOST_AUTO_TEST_CASE(BeginReadOnUnlockedRecordReturnsTrue) {
    RecordLockManager manager;
    bool result = manager.beginRead();
    if (result) manager.endRead();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginWriteOnUnlockedRecordReturnsTrue) {
    RecordLockManager manager;
    bool result = manager.beginWrite();
    if (result) manager.endWrite();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginWriteWhileReadLockExistsReturnsFalse) {
    RecordLockManager manager;
    manager.beginRead();
    bool result = !manager.beginWrite();
    manager.endRead();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(BeginReadWhileWriteLockExistsReturnsFalse) {
    RecordLockManager manager;
    manager.beginWrite();
    bool result = !manager.beginRead();
    manager.endWrite();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(HasWriterReflectsWriteState) {
    RecordLockManager manager;
    bool before = manager.hasWriter();
    manager.beginWrite();
    bool during = manager.hasWriter();
    manager.endWrite();
    bool result = !before && during;
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(EndReadWithoutBeginReturnsFalse) {
    RecordLockManager manager;
    bool result = !manager.endRead();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_CASE(EndWriteWithoutBeginReturnsFalse) {
    RecordLockManager manager;
    bool result = !manager.endWrite();
    BOOST_CHECK(result);
}

BOOST_AUTO_TEST_SUITE_END()
