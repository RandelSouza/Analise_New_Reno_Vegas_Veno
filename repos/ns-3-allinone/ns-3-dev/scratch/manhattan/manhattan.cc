/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation;
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Author: Josh Pelkey <jpelkey@gatech.edu>
*/
#include "ns3/random-variable-stream.h"
#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include <string>
#include <fstream>
#include "ns3/packet-sink.h"
#include "ns3/error-model.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-header.h"
#include "ns3/enum.h"
#include "ns3/event-id.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "ns3/command-line.h"
#include "ns3/random-variable-stream.h"
#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("manhathan");

int main (int argc, char *argv[])
{

    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();

    uint32_t xSize = 5;
    uint32_t ySize = 5;
    float errorRate = 0.5;
    std::string typeTCP = "1";        
    std::string nomeTCP = "";
    uint32_t dataRate = 0;
    std::string count = "0";

    CommandLine cmd;
    cmd.AddValue ("xSize", "Number of rows of nodes", xSize);
    cmd.AddValue ("ySize", "Number of columns of nodes", ySize);
    cmd.AddValue ("typeTCP", "Versão do TCP, podendo ser NewReno será representado pelo valor 1, Vegas será representado pelo valor 2, Veno será representado pelo valor 3", typeTCP);
    cmd.AddValue ("errorRate", "taxa de erro na simulação ex.: 0.001", errorRate);                                                         
    cmd.AddValue ("dataRate", "Taxa de trasmissão", dataRate);    
    cmd.AddValue ("count", "contador", count);  

    //cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);
    cmd.Parse (argc,argv);     
    
    Config::SetDefault ("ns3::RateErrorModel::ErrorRate", DoubleValue (errorRate));
    Config::SetDefault ("ns3::RateErrorModel::ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));
    NS_LOG_INFO (errorRate);

    Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (1024));
    Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("1000kb/s"));    
            
    // Configurando a versão do tcp utilizado    
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

    std::string animFile = "manhattan-"+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+std::to_string(dataRate)+".xml";
        
    PointToPointHelper pointToPoint;
    //pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue (std::to_string(dataRate)+"Mbps"));
    //NS_LOG_INFO (std::to_string(dataRate)+"Mbps");        

   // NS_LOG_INFO(value);
    //Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();     
    //std::cout << uv->GetValue () << std::endl;
    
    double min = 1.0;
    double max = 5.0+xSize;
    double value = 0.0;
    Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
    uv->SetAttribute ("Min", DoubleValue (min));                
    uv->SetAttribute ("Max", DoubleValue (max));
    // The expected value for the mean of the values returned by a
    // normally distributed random variable is equal to mean.    
    if(count.compare("1") == 0){
        for(uint32_t i = 0; i < 1000+ySize; i++){
            RngSeedManager::SetSeed (12+i);
            RngSeedManager::SetRun(101-i);
            RngSeedManager::SetSeed (12+i);
            RngSeedManager::SetRun(101-i);
            value =  uv->GetValue ()+uv->GetValue ()-3;
            //std::cout << uv->GetValue ()+uv->GetValue ()-3 << std::endl;
        }   
    }else{
        for(uint32_t i = 0; i < 2000+xSize; i++){    
            RngSeedManager::SetSeed (12+i);
            RngSeedManager::SetRun(101-i);
            RngSeedManager::SetSeed (12+i);
            RngSeedManager::SetRun(101-i);
            value =  uv->GetValue ()+uv->GetValue ()-3;
            //std::cout << uv->GetValue ()+uv->GetValue ()-3 << std::endl;
        }
    }
        
    
    pointToPoint.SetChannelAttribute ("Delay", StringValue(std::to_string(value-1)+"2ms"));

    //NS_LOG_INFO ("taxa de transmissão: " + std::to_string(dataRate));
    // Create Grid
    PointToPointGridHelper grid (xSize, ySize, pointToPoint);

    // Install stack on Grid
    InternetStackHelper stack;
    grid.InstallStack (stack);

    // Assign Addresses to Grid
    grid.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                              Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"));                    
      
    OnOffHelper clientHelper ("ns3::UdpSocketFactory", Address ());
    clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=10]"));
    clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=20]"));
    ApplicationContainer clientApps;
    
    // Create an on/off app sending packets
    AddressValue remoteAddress (InetSocketAddress (grid.GetIpv4Address (xSize-1,ySize-1), 1000));
    clientHelper.SetAttribute ("Remote", remoteAddress);


    BulkSendHelper ftp("ns3::TcpSocketFactory", Address ());
    ftp.SetAttribute ("Remote", remoteAddress);
    ftp.SetAttribute ("SendSize", UintegerValue (512)); 
    ftp.SetAttribute ("MaxBytes", UintegerValue (0)); 
    

    for(uint32_t x = 0; x < xSize ; x++){
        for(uint32_t y = 0; y < ySize; y++){
    
            clientApps.Add (ftp.Install(grid.GetNode (x,y)));            
        }
    }
      
    clientApps.Start (Seconds (1.0));
    clientApps.Stop (Seconds (10.0+value));
    
    // Set the bounding box for animation
    grid.BoundingBox (1, 1, 100, 100);
    
    //Create the animation object and configure for specified output
    AnimationInterface anim (animFile);
    anim.EnablePacketMetadata (); // Optional
    anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (10)); // Optional  

    // Set up the actual simulation
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    pointToPoint.EnablePcapAll ("manhattan");   

    //Ptr<FlowMonitor> flowMonitor;
    //FlowMonitorHelper flowHelper;
    //flowMonitor = flowHelper.InstallAll();

    AsciiTraceHelper ascii;
  
    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("manhattan-"+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+std::to_string(dataRate)+".tr"));    
    NS_LOG_INFO ("Rodando a simulação.");
    
    Simulator::Run ();        

    /*Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper.GetClassifier ());
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
    flowMonitor->SerializeToXmlFile("data.flowmon", true, true);  */
    Simulator::Destroy ();

    return 0;
}