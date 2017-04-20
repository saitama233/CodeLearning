from twisted.internet import reactor
from twisted.internet.protocol import Protocol,Factory
from twisted.protocols.basic import LineReceiver

class SimpleLogger(LineReceiver):
    def connectionMade(self):
        print 'Got connection from',self.transport.client

    def connectionLost(self, reason):
        print self.transport.client,' disconnected'

    def dataReceived(self, line):
        print line

factory = Factory()
factory.protocol = SimpleLogger

reactor.listenTCP(9527,factory)
reactor.run()