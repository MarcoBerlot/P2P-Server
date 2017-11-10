#include "main.hh"


ChatDialog::ChatDialog()
{

	// Read-only text box where we display messages from everyone.
	// This widget expands both horizontally and vertically.
	textview = new QTextEdit(this);
	textview->setReadOnly(true);
	counter=1;
	srand(time(0));
	// Small text-entry box the user can enter messages.
	// This widget normally expands only horizontally,
	// leaving extra vertical space for the textview widget.
	//
	// You might change this into a read/write QTextEdit,
	// so that the user can easily enter multi-line messages.
	textline = new QLineEdit(this);
	QVariantMap nested;
	status["Want"]=QVariantMap(nested);

	// Lay out the widgets to appear in the main window.
	// For Qt widget and layout concepts see:
	// http://doc.qt.nokia.com/4.7-snapshot/widgets-and-layouts.html
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(textview);
	layout->addWidget(textline);
	setLayout(layout);

	socket = new NetSocket();
    if (!socket->bind())
        exit(1);
    qDebug() << "Bound with (from) main" <<socket->port;
    // qDebug() << socket.myPortMin <<"bingo";

    setWindowTitle(socket->portInfo);
	// Register a callback on the textline's returnPressed signal
	// so that we can send the message entered by the user.
	// udpSocket.bind(36768);
	quint32 rndm=rand() % 4;
	myName=QVariant(rndm).toString()+QHostInfo::localHostName();

	qDebug() << myName;
	connect(textline, SIGNAL(returnPressed()),
	this, SLOT(gotReturnPressed()));

	connect(socket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repeatMessage()));
	antiEntropyTimer = new QTimer(this);
	connect(antiEntropyTimer, SIGNAL(timeout()), this, SLOT(processAntiEntropy()));
  antiEntropyTimer->start(7000);
}

void ChatDialog::gotReturnPressed()
{
	// Initially, just echo the string locally.
	// Insert some networking code here...
	QVariantMap map;
	QVariantMap newMessage;
	QByteArray datagram;
	qint64 value=0;


	QVariantMap nested=qvariant_cast<QVariantMap>(status["Want"]);
	QString index= QVariant(counter).toString();
	QString port=QVariant(socket->port).toString();
	QString text=QVariant(textline->text()).toString();
	//Create a VariantMap
	map["ChatText"]=text;
	map["Origin"]=myName;
	map["SeqNo"]=index;
	//ADD the new message to the status message and to the old messages

	if (counter<2){
	newMessage[QVariant(counter).toString()]=QVariant(text);
	oldMessagesCollection[myName]=QVariant(newMessage);
}else{
	oldEntry=qvariant_cast<QVariantMap>(oldMessagesCollection[map["Origin"].toString()]);
	oldEntry[QVariant(counter).toString()]=QVariant(text);
	oldMessagesCollection[map["Origin"].toString()]=QVariant(oldEntry);
}
	counter++;
	nested[map["Origin"].toString()]=QVariant(counter);
	status["Want"]=QVariant(nested);
	qDebug()<<"Message Collection";
	qDebug()<<oldMessagesCollection;


	//Creates Stream
	QDataStream outStream(&datagram, QIODevice::WriteOnly);
	outStream << map;

	int portToSend = 0;
	//pick neighbor
 	if (socket->port == socket->myPortMin) {
        portToSend= socket->myPortMin + 1;
    }
    else if (socket->port ==  socket->myPortMax) {
        portToSend= socket->myPortMax - 1;
    }
    else {
    portToSend= qrand() % 2 == 1?  socket->port - 1 : socket->port + 1;
	}
	timer->start(5000);
	ackPort=portToSend;
	ackMessage=map;
	qDebug()<<"sending to "<<portToSend;


    value=socket->writeDatagram(datagram, QHostAddress("127.0.0.1"), portToSend);


	textview->append(socket->portInfo + " : " + map["ChatText"].toString() + " sent to: " + QString::number(portToSend));


	textview->append(map["ChatText"].toString());


	// Clear the textline to get ready for the next input message.
	textline->clear();

}

// quint32 ChatDialog::chooseNeighbor(quint32 port){
// 	if (port == socket->myPortMin){
// 		return port+1;
// 	}
// 	else if (port == socket->myPortMax){
// 		return port-1;
// 	}
// 	else{
// 		r = ((double) rand() / (RAND_MAX)) + 1
// 		if (r==1){
// 			return port+1;
// 		}
// 		else {
// 			return port-1;
// 		}
// 	}
// }


void ChatDialog::processPendingDatagrams()

{
  QByteArray datagram;
	QVariantMap inMap;
	QHostAddress address;
	quint16 port;
	QVariantMap nested=qvariant_cast<QVariantMap>(status["Want"]);

    do {
		timer->stop();
		//Receive the datagram
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(),&address, &port);

		QDataStream inStream(&datagram, QIODevice::ReadOnly);
		inStream >> inMap;


		//check if is a status(want) or rumor
		if (inMap.contains("Want")) {
			QMap<QString, QVariant> neighborMap = inMap["Want"].toMap();
            processStatus(neighborMap, port);
        } else {
        	 processRumor(inMap,port);
        	 QVariantMap nested=qvariant_cast<QVariantMap>(status["Want"]);
        }

	    qDebug() << "my Status message"<< status;
		qDebug() << "my Message Collection"<< oldMessagesCollection;
		} while (socket->hasPendingDatagrams());
}


void ChatDialog::processRumor(QVariantMap inMap, quint16 port)

{
	qDebug() << "Received Rumor";
	QVariantMap nested=qvariant_cast<QVariantMap>(status["Want"]);
	//Append to view
	textview->append("Received from:" + inMap["Origin"].toString());
	textview->append("Content:" + inMap["ChatText"].toString());
	//Change status
	//FIX IF STATEMENT, CONDITION IS ALWAYS TRUE

	int flag=0;
	for(QVariantMap::const_iterator iter = nested.begin(); iter != nested.end(); ++iter) {
		//Receiver already in the status message
			if(iter.key().compare(inMap["Origin"].toString())==0) {
				int tmp=nested[iter.key()].toInt();
				if(tmp!=inMap["SeqNo"].toInt()){
						flag=1;
						//Drop the packet
						break;
					}else{
				nested[iter.key()]=QVariant(++tmp);
				flag=1;
				oldEntry=qvariant_cast<QVariantMap>(oldMessagesCollection[inMap["Origin"].toString()]);
				oldEntry[inMap["SeqNo"].toString()]=QVariant(inMap["ChatText"].toString());
				oldMessagesCollection[inMap["Origin"].toString()]=QVariant(oldEntry);
				}
			}
		}
		//New receiver
		if(flag==0){
			nested[inMap["Origin"].toString()]=QVariant(1);
			QVariantMap newMessage;
			newMessage[inMap["SeqNo"].toString()]=QVariant(inMap["ChatText"].toString());
			oldMessagesCollection[inMap["Origin"].toString()]=QVariant(newMessage);
		}
		status["Want"]=QVariant(nested);
		sendStatus(port);
}

void ChatDialog::processStatus(QMap<QString, QVariant> neighborMap , quint16 port) {
	// qDebug() << "ProcessStatus"<< neighborMap;
	QVariantMap nested=qvariant_cast<QVariantMap>(status["Want"]);
	qDebug() << "Received Status";

    // check my values
    for (QVariantMap::const_iterator iter = nested.begin(); iter != nested.end(); ++iter) {
        QString Origin = iter.key();
        quint32 seqNo =  iter.value().toUInt();

        // case1- I need to share info
        if (!neighborMap.contains(Origin) || neighborMap[Origin].toUInt() < seqNo) {

            quint32 indexToSend = 1;
            if (!neighborMap.contains(Origin)) {
                indexToSend = 1;
            } else {
                indexToSend = neighborMap[Origin].toUInt();
            }

			QString index= QVariant(indexToSend).toString();
            //send origin and indexToSend
						sendRumor(Origin,index, port);
            return;

        }
    }
    // check neighbor's Map
    for (QVariantMap::const_iterator iter = neighborMap.begin(); iter != neighborMap.end(); ++iter) {
        QString neighborOrigin = iter.key();
        quint32 neighborSeqNo =  iter.value().toUInt();

        // case2 - I need to receive info
        if (!nested.contains(neighborOrigin) || nested[neighborOrigin].toUInt() < neighborSeqNo) {
        	//Send Status
					sendStatus(port);
          return;
        }
    }

    return;


}
void ChatDialog::sendRumor(QString myOrigin,QString mySeqNo, quint16 myPort){
	QVariantMap map;
	QByteArray datagram;
	QString Origin=myOrigin;
	QString seqNo=mySeqNo;
	quint16 port=myPort;
	QVariantMap tmp=qvariant_cast<QVariantMap>(oldMessagesCollection[Origin]);
	qDebug()<<"Sending missinng Rumor";
	QString text=tmp[seqNo].toString();
	qDebug()<<seqNo.toInt();

	map["ChatText"]=QVariant(text);
	map["Origin"]=QVariant(Origin);
	map["SeqNo"]=QVariant(seqNo);

	//Creates Stream
	QDataStream outStream(&datagram, QIODevice::WriteOnly);
	outStream << map;
	timer->start(5000);
	ackMessage=map;
	ackPort;
  socket->writeDatagram(datagram, QHostAddress("127.0.0.1"), port);

}
void ChatDialog::sendStatus(quint16 myPort){
	QByteArray datagram;
	QDataStream outStream(&datagram, QIODevice::WriteOnly);
	outStream << status;
	socket->writeDatagram(datagram, QHostAddress("127.0.0.1"), myPort);
}

void ChatDialog::processAntiEntropy() {
    qDebug() << "Anti Entropy";
    antiEntropyTimer->start(7000);

	int portToSend = 0;

    if (socket->port == socket->myPortMin) {
        portToSend= socket->myPortMin + 1;
    }
    else if (socket->port ==  socket->myPortMax) {
        portToSend= socket->myPortMax - 1;
    }
    else {
    portToSend= qrand() % 2 == 1?  socket->port - 1 : socket->port + 1;
	}
	sendStatus(portToSend);
	return;


	//process status
}

void ChatDialog::repeatMessage(){
	qDebug() << " Resending the message";
	QByteArray datagram;
	QDataStream outStream(&datagram, QIODevice::WriteOnly);
	outStream << ackMessage;
	socket->writeDatagram(datagram, QHostAddress("127.0.0.1"), ackPort);

}
NetSocket::NetSocket()
{

	// Pick a range of four UDP ports to try to allocate by default,
	// computed based on my Unix user ID.
	// This makes it trivial for up to four P2Papp instances per user
	// to find each other on the same host,
	// barring UDP port conflicts with other applications
	// (which are quite possible).
	// We use the range from 32768 to 49151 for this purpose.
	// myPortMin = 32768 + (getuid() % 4096)*4;
	// myPortMax = myPortMin + 3;
}

bool NetSocket::bind()
{
	// Try to bind to each of the range myPortMin..myPortMax in turn.
	for (int p = myPortMin; p <= myPortMax; p++) {
		if (QUdpSocket::bind(p)) {
			qDebug() << "bound to UDP port " << p;
			portInfo.append(QString("Port "));
            portInfo.append(QString::number(p));
			qDebug() << "this is ID " << portInfo;
			port = p;
			return true;
		}
	}


	qDebug() << "Oops, no ports in my default range " << myPortMin
		<< "-" << myPortMax << " available";
	return false;
}


int main(int argc, char **argv)
{
	// Initialize Qt toolkit
	QApplication app(argc,argv);

	// Create an initial chat dialog window
	ChatDialog dialog;
	dialog.show();

	// Create a UDP network socket
	// NetSocket sock;
	// if (!sock.bind())
	// 	exit(1);

	// Enter the Qt main loop; everything else is event driven
	return app.exec();
}
