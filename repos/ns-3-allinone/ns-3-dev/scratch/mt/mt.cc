/*
MIT License
Copyright (c) 2017 Sambhav Kothari
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Network topology
//
//       n0 ----------- n1
//            1 Mbps
//             10 ms
//
// - Flow from n0 to n1 using BulkSendApplication.



// This code heavily borrows from ns3 itself which are copyright of their
// respective authors and redistributable under the same conditions.

#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/error-model.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/enum.h"
#include "ns3/event-id.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TcpComparision");

AsciiTraceHelper ascii;
Ptr<PacketSink> cbrSinks[5], tcpSink;

int totalVal;
int total_drops=0;
bool first_drop=true;

// Function to record packet drops
static void
RxDrop (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
    if(first_drop)
    {
        first_drop=false;
        *stream->GetStream ()<<0<<" "<<0<<std::endl;
    }
    *stream->GetStream ()<<Simulator::Now ().GetSeconds ()<<" "<<++total_drops<<std::endl;
}


// Function to find the total cumulative recieved bytes
static void
TotalRx(Ptr<OutputStreamWrapper> stream)
{
    totalVal = tcpSink->GetTotalRx();

    for(int i=0; i<5; i++)
    {
        totalVal += cbrSinks[i]->GetTotalRx();
    }

    *stream->GetStream()<<Simulator::Now ().GetSeconds ()<<" " <<totalVal<<std::endl;

    Simulator::Schedule(Seconds(0.0001),&TotalRx, stream);
}

// Function to record Congestion Window values
static void
CwndChange (Ptr<OutputStreamWrapper> stream, uint32_t oldCwnd, uint32_t newCwnd)
{
    *stream->GetStream () << Simulator::Now ().GetSeconds () << " " << newCwnd << std::endl;
}

//Trace Congestion window length
static void
TraceCwnd (Ptr<OutputStreamWrapper> stream)
{
    //Trace changes to the congestion window
    Config::ConnectWithoutContext ("/NodeList/0/$ns3::TcpL4Protocol/SocketList/0/CongestionWindow", MakeBoundCallback (&CwndChange,stream));
}

int main (int argc, char *argv[]){    

    bool tracing = true;
    uint32_t maxBytes = 0;
    std::string typeTCP = "1";
    double error = 0.5;        
    std::string n = "10.0";
    std::string m = "1.0";
    std::string dataRate = "10";
    std::string nomeTCP = "";
    uint32_t xSize = 5;
    uint32_t ySize = 5;
    float errorRate = 0.5;    
    std::string count = "0";

    CommandLine cmd;
    cmd.AddValue ("tracing", "Flag to enable/disable tracing", tracing);
    cmd.AddValue ("maxBytes",
                  "Total number of bytes for application to send", maxBytes);
    cmd.AddValue ("typeTCP", "Transport protocol to use: TcpNewReno, "
                  "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                  "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus ", typeTCP);
    cmd.AddValue ("n", "number", n);
    
    cmd.AddValue ("xSize", "Number of rows of nodes", xSize);
    cmd.AddValue ("ySize", "Number of columns of nodes", ySize);
    cmd.AddValue ("typeTCP", "Versão do TCP, podendo ser NewReno será representado pelo valor 1, Vegas será representado pelo valor 2, Veno será representado pelo valor 3", typeTCP);
    cmd.AddValue ("errorRate", "taxa de erro na simulação ex.: 0.001", errorRate);                                                         
    cmd.AddValue ("dataRate", "Taxa de trasmissão", dataRate);    
    cmd.AddValue ("count", "contador", count);  

    cmd.AddValue ("m", "number", m);

    cmd.Parse (argc, argv);

    if(typeTCP.compare("1") == 0){                      
        NS_LOG_INFO ("O tipo é " + typeTCP + " New Reno");
        nomeTCP = "New Reno";
        Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpNewReno::GetTypeId ()));

    }else if(typeTCP.compare("2") == 0){        
        NS_LOG_INFO ("O tipo é " + typeTCP + " Vegas");
        nomeTCP = "Vegas";
        Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpVegas::GetTypeId ()));

    }else if(typeTCP.compare("3") == 0){        
        NS_LOG_INFO ("O tipo é " + typeTCP+ " Veno");
        nomeTCP = "Veno";
        Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpVeno::GetTypeId ()));

    }else{
        NS_LOG_DEBUG ("Versão inválida de TCP!");
        exit (1);
    }
    std::string a_s = "bytes_"+nomeTCP+".dat";
    std::string b_s = "drop_"+nomeTCP+".dat";
    std::string c_s = "cw_"+nomeTCP+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+dataRate+".dat";

    std::string animFile = "manhattan-"+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+dataRate+".xml";

    // Create file streams for data storage
    Ptr<OutputStreamWrapper> total_bytes_data = ascii.CreateFileStream (a_s);
    Ptr<OutputStreamWrapper> dropped_packets_data = ascii.CreateFileStream (b_s);
    Ptr<OutputStreamWrapper> cw_data = ascii.CreateFileStream (c_s);

    // Explicitly create the nodes required by the topology (shown above).
    NS_LOG_INFO ("Create nodes.");
    NodeContainer nodes;
    nodes.Create (2);

    NS_LOG_INFO ("Create channels.");

    // Explicitly create the point-to-point link required by the topology (shown above).
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue (dataRate+"Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue (m+"ms"));
    pointToPoint.SetQueue ("ns3::DropTailQueue");

    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes);

    // Revisar para colocar a taxa de error adequada
    Ptr<RateErrorModel> em =
    CreateObjectWithAttributes<RateErrorModel> ("RanVar", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max="+n+"]"),
                                                "ErrorRate", DoubleValue (error),
                                                "ErrorUnit", EnumValue (RateErrorModel::ERROR_UNIT_PACKET)
                                                );
    devices.Get (1)->SetAttribute ("ReceiveErrorModel", PointerValue (em));
   

    // Install the internet stack on the nodes
    InternetStackHelper internet;
    internet.Install (nodes);

    // We've got the "hardware" in place.  Now we need to add IP addresses.
    NS_LOG_INFO ("Assign IP Addresses.");
    Ipv4AddressHelper ipv4;
    ipv4.SetBase ("10.1.1.0", "255.255.255.0");

    NS_LOG_INFO ("Create Applications.");

    // Create a BulkSendApplication and install it on node 0
    uint16_t port = 12344;
    Ipv4InterfaceContainer ipv4Container = ipv4.Assign (devices)
    ;
    
    BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (ipv4Container.GetAddress (1), port));
    // Set the amount of data to send in bytes.  Zero is unlimited.
    source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
    
    ApplicationContainer sourceApps;
    
    sourceApps.Add(source.Install(nodes.Get (0)));
    //sourceApps.Add(source.Install(nodes.Get (2)));
    
    
    sourceApps.Start (Seconds (0.0));
    sourceApps.Stop (Seconds (1.80));

    // Create a PacketSinkApplication and install it on node 1
    PacketSinkHelper sink ("ns3::TcpSocketFactory",
                           InetSocketAddress (Ipv4Address::GetAny (), port));
    ApplicationContainer sinkApps = sink.Install (nodes.Get (1));


    sinkApps.Start (Seconds (0.0));
    sinkApps.Stop (Seconds (1.80));

    tcpSink = DynamicCast<PacketSink> (sinkApps.Get (0));


    uint16_t cbrPort = 12345;

    double startTimes[5] = {0.2, 0.4, 0.6, 0.8, 1.0};
    double endTimes[5]   = {1.8, 1.8, 1.2, 1.4, 1.6};

    for(int i=0; i<5; i++)
    {
        // Install applications: five CBR streams each saturating the channel
        ApplicationContainer cbrApps;
        ApplicationContainer cbrSinkApps;
        OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (ipv4Container.GetAddress (1), cbrPort+i));
        onOffHelper.SetAttribute ("PacketSize", UintegerValue (1024));
        onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
        onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

        onOffHelper.SetAttribute ("DataRate", StringValue ("300Kbps"));
        onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (startTimes[i])));
        onOffHelper.SetAttribute ("StopTime", TimeValue (Seconds (endTimes[i])));
        cbrApps.Add (onOffHelper.Install (nodes.Get (0)));
        // Packet sinks for each CBR agent

        PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), cbrPort+i));
        cbrSinkApps = sink.Install (nodes.Get (1));
        cbrSinkApps.Start (Seconds (0.0));
        cbrSinkApps.Stop (Seconds (1.8));
        cbrSinks[i] = DynamicCast<PacketSink> (cbrSinkApps.Get (0));
    }

    devices.Get (1)->TraceConnectWithoutContext ("PhyRxDrop", MakeBoundCallback (&RxDrop, dropped_packets_data));

    
    AsciiTraceHelper ascii;
    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("tcp-comparision.tr"));
    pointToPoint.EnablePcapAll ("tcp-comparision", true);


    NS_LOG_INFO ("Run Simulation.");

    Simulator::Schedule(Seconds(0.00001),&TotalRx, total_bytes_data);
    Simulator::Schedule(Seconds(0.00001),&TraceCwnd, cw_data);
    
    //Create the animation object and configure for specified output
    //AnimationInterface anim (animFile);
    //anim.EnablePacketMetadata (); // Optional
    //anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (100)); // Optional  

    // Flow monitor
    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    Simulator::Stop (Seconds (1.80));
    Simulator::Run ();
    
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();
    
    std::cout << std::endl << "*** Flow monitor statistics ***" << std::endl;
    std::cout << "  Tx Packets:   " << stats[1].txPackets << std::endl;
    std::cout << "  Tx Bytes:   " << stats[1].txBytes << std::endl;
    std::cout << "  Offered Load: " << stats[1].txBytes * 8.0 / (stats[1].timeLastTxPacket.GetSeconds () - stats[1].timeFirstTxPacket.GetSeconds ()) / 1000000 << " Mbps" << std::endl;
    std::cout << "  Rx Packets:   " << stats[1].rxPackets << std::endl;
    std::cout << "  Rx Bytes:   " << stats[1].rxBytes<< std::endl;
    std::cout << "  Throughput: " << stats[1].rxBytes * 8.0 / (stats[1].timeLastRxPacket.GetSeconds () - stats[1].timeFirstRxPacket.GetSeconds ()) / 1000000 << " Mbps" << std::endl;
    std::cout << "  Mean delay:   " << stats[1].delaySum.GetSeconds () / stats[1].rxPackets << std::endl;
    std::cout << "  Mean jitter:   " << stats[1].jitterSum.GetSeconds () / (stats[1].rxPackets - 1) << std::endl;    
    // não esquecer de enviar os outros dados para fazer os calculos no R
    flowMonitor->SerializeToXmlFile("data02.flowmon", true, true);    

    Simulator::Destroy ();
    NS_LOG_INFO ("Done.");
}