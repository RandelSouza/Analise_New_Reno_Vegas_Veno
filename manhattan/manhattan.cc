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

#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("manhathan");

int main (int argc, char *argv[])
{   
    ns3::RngSeedManager::SetSeed(10);
    // Para calcular o tamanho a ser enviado 
    Time::SetResolution (Time::NS);

    uint64_t data_mbytes = 1024;
    uint32_t mtu_bytes = 400;
     
    // Calculate the ADU size
    Header* temp_header = new Ipv4Header ();
    uint32_t ip_header = temp_header->GetSerializedSize ();
    
    delete temp_header;

    temp_header = new TcpHeader ();
    uint32_t tcp_header = temp_header->GetSerializedSize ();
    
    delete temp_header;

    uint32_t tcp_adu_size = mtu_bytes - 20 - (ip_header + tcp_header);                    

    uint32_t xSize = 5;
    uint32_t ySize = 5;
    float errorRate = 0.2;
    std::string typeTCP = "1";        
    std::string nomeTCP = "";
    uint32_t dataRate = 0;

    CommandLine cmd;
    cmd.AddValue ("xSize", "Number of rows of nodes", xSize);
    cmd.AddValue ("ySize", "Number of columns of nodes", ySize);
    cmd.AddValue ("typeTCP", "Versão do TCP, podendo ser NewReno será representado pelo valor 1, Vegas será representado pelo valor 2, Veno será representado pelo valor 3", typeTCP);
    cmd.AddValue ("errorRate", "taxa de erro na simulação ex.: 0.001", errorRate);                                                         
    cmd.AddValue ("dataRate", "Taxa de trasmissão", dataRate);    


    //cmd.AddValue ("animFile",  "File Name for Animation Output", animFile);
    cmd.Parse (argc,argv);

    
    if (xSize < 1 || ySize < 1 || (xSize < 2 && ySize < 2))
    {
        NS_FATAL_ERROR ("São necessários mais nós para criar a topologia manhathan.");
    }
    
    
    Config::SetDefault ("ns3::RateErrorModel::ErrorRate", DoubleValue (errorRate));
    Config::SetDefault ("ns3::RateErrorModel::ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));
    NS_LOG_INFO (errorRate);

    Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (512));
    Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("500kb/s"));    

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
    NS_LOG_INFO (std::to_string(dataRate)+"Mbps");
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

    NS_LOG_INFO ("taxa de transmissão: " + std::to_string(dataRate));
    // Create Grid
    PointToPointGridHelper grid (xSize, ySize, pointToPoint);

    // Install stack on Grid
    InternetStackHelper stack;
    grid.InstallStack (stack);

    // Assign Addresses to Grid
    grid.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                              Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"));                    
      
    OnOffHelper clientHelper ("ns3::UdpSocketFactory", Address ());
    clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
    ApplicationContainer clientApps;
    
    // Create an on/off app sending packets
    AddressValue remoteAddress (InetSocketAddress (grid.GetIpv4Address (xSize-1,ySize-1), 1000));
    clientHelper.SetAttribute ("Remote", remoteAddress);


    BulkSendHelper ftp("ns3::TcpSocketFactory", Address ());
    ftp.SetAttribute ("Remote", remoteAddress);
    ftp.SetAttribute ("SendSize", UintegerValue (tcp_adu_size)); // não é visivel 
    NS_LOG_INFO (std::to_string(tcp_adu_size));
    ftp.SetAttribute ("MaxBytes", UintegerValue (data_mbytes * 1000000)); // verificar
    NS_LOG_INFO (std::to_string(data_mbytes * 1000000));

    for(uint32_t x = 0; x < xSize ; x++){
        for(uint32_t y = 0; y < ySize; y++){
    //      printf("%i %i\n"-, x, y);                
            //clientApps.Add (clientHelper.Install (grid.GetNode (x,y)));
            clientApps.Add (ftp.Install(grid.GetNode (x,y)));            
        }
    }
      
    clientApps.Start (Seconds (1.0));
    clientApps.Stop (Seconds (2.0));
    
    // Set the bounding box for animation
    grid.BoundingBox (1, 1, 100, 100);
    
    //Create the animation object and configure for specified output
    AnimationInterface anim (animFile);
    anim.EnablePacketMetadata (); // Optional
    anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (10)); // Optional  

    // Set up the actual simulation
    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
    pointToPoint.EnablePcapAll ("manhattan");   


    AsciiTraceHelper ascii;

    pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("manhattan-"+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+std::to_string(dataRate)+".tr"));    
    NS_LOG_INFO ("Rodando a simulação.");
    Simulator::Run ();    
    
    Simulator::Destroy ();

    return 0;
}