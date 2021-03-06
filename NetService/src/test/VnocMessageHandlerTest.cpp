#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../VnocMessageSocketHandler.hpp"
#include "../VnocProtocol.hpp"
#include "MockTcpConnection.hpp"
#include "../RvcMessageHandler.hpp"
#include "../RliMessageHandler.hpp"
#include "../RclMessageHandler.hpp"
#include "../FileUserStorage.h"
#include "../UserManage.hpp"
#include "../../Message/MSG_UNION.h"
#include "../../Message/PackMessage.h"

class VnocMessageHandlerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( VnocMessageHandlerTest );
    CPPUNIT_TEST( testRVC );
    CPPUNIT_TEST( testRLI );
    CPPUNIT_TEST( testRCL );
    CPPUNIT_TEST_SUITE_END();
    MockTcpConnection *conn_;
    VnocProtocol *protocol_;
public:
	fUserStorage us;

    void setUp()
    {
        conn_ = new MockTcpConnection;
        protocol_ = new VnocProtocol();
		CUserManage::GetInstance()->initial(&us);
    }
    void tearDown()
    {
        delete protocol_;
    }
public:
    void testRVC()
    {
        RvcMessageHandler rvcHandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        char rvc[]={0x56,
            0x00,
            0x00,0x01,
            0x00,0x00,0x00,0x35,

            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,

            0x14,
            0x00,0x00,0x00,0x00,
            0x01,

            0x00,0x00,0x00,0x10,

            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,

            0x00,0x00,
            0x43};
        conn_->setRecv(rvc, sizeof(rvc));
        const char *sendBuf = conn_->getSendBuf();
        //return a AVC message
        CPPUNIT_ASSERT(sendBuf[24]==0x15);
    }
    void testRLI()
    {
        char testRLI [] = {0x56,
            0x00,
            0x00,0x01,
            0x00,0x00,0x00,0x5D,


            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,

            0x16,
            0x00,0x00,0x00,0x00,

            0x03,

            0x00,0x00,0x00,0x10,
            0x00,0x00,0x00,0x10,
            0x00,0x00,0x00,0x10,


            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,
            0x02,0x02,0x02,0x02,

            0x03,0x03,0x03,0x03,
            0x03,0x03,0x03,0x03,
            0x03,0x03,0x03,0x03,
            0x03,0x03,0x03,0x03,

            0x04,0x04,0x04,0x04,
            0x04,0x04,0x04,0x04,
            0x04,0x04,0x04,0x04,
            0x04,0x04,0x04,0x04,

            0x00,0x00,
            0x43};
        RliMessageHandler rvcHandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        conn_->setRecv(testRLI, sizeof(testRLI));
        const char *sendBuf = conn_->getSendBuf();
        //return an AVC message
        CPPUNIT_ASSERT(sendBuf[24]==0x17);
    }
    void testRCL()
    {
        RclMessageHandler rclhandler(protocol_);
        VnocMessageSocketHandler<MockTcpConnection> handler(conn_);
        handler.setProtocol(protocol_);
        handler.start();
        MSG_RCL rclMessage;
        PackMessage packer;
        int len = packer.GetMessageLen(&rclMessage);
        char *buf = new char[len];
        packer.Pack(&rclMessage, (byte *)buf, len);
        conn_->setRecv(buf, len);
        const char *sendBuf = conn_->getSendBuf();
        //return an ACL message
        CMessageParser parser;
        CMessage *msg = parser.Parse((byte*)sendBuf, conn_->getSendLen());
        CPPUNIT_ASSERT(msg->GetMessageType()==MSG_ACL_TYPE);
        delete msg;
        delete buf;
    }

};
CPPUNIT_TEST_SUITE_REGISTRATION ( VnocMessageHandlerTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VnocMessageHandlerTest, "VnocMessageHandlerTest");
