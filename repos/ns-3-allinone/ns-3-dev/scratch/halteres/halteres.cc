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
 * Author: George F. Riley<riley@ece.gatech.edu>
 */

#include <iostream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/tcp-vegas.h"
#include "ns3/log.h"
#include "ns3/tcp-congestion-ops.h"
#include "ns3/tcp-socket-base.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("halteres");

int main (int argc, char *argv[])
{
  ns3::RngSeedManager::SetSeed(10);

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (1024));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("500kb/s"));

  // Variaveis padrão caso nenhum valor seja passado como argumento pelo terminal (Linha de Comando do inglês Command Line cmd)
  uint32_t    nLeftLeaf = 5;
  uint32_t    nRightLeaf = 5;
  uint32_t    nLeaf = 0; // If non-zero, number of both left and right
  std::string nomeTCP = "" ;  // Nome da versão do TCP utilizada
  float errorRate = 0.2;
  std::string typeTCP = "1";
  uint32_t dataRate = 0;


  // Aqui se declara quais variaveis podem ser passadas como parametro pela linha de comando
  // O primeiro argumento é o nome do paramétro, o segundo é a descrição e o terceiro argumento
  // É onde realmente o valor será armazenado
  CommandLine cmd;
  cmd.AddValue ("nLeftLeaf", "Number of left side leaf nodes", nLeftLeaf);
  cmd.AddValue ("nRightLeaf","Number of right side leaf nodes", nRightLeaf);
  cmd.AddValue ("nLeaf",     "Number of left and right side leaf nodes", nLeaf);
  cmd.AddValue ("typeTCP", "Versão do TCP, podendo ser NewReno será representado pelo valor 1, Vegas será representado pelo valor 2, Veno será representado pelo valor 3", typeTCP);
  cmd.AddValue ("errorRate", "taxa de erro na simulação ex.: 0.001", errorRate);
  cmd.AddValue ("dataRate", "Taxa de trasmissão", dataRate);
  cmd.Parse (argc,argv);


  Config::SetDefault ("ns3::RateErrorModel::ErrorRate", DoubleValue (errorRate));
  Config::SetDefault ("ns3::RateErrorModel::ErrorUnit", StringValue ("ERROR_UNIT_PACKET"));
  NS_LOG_INFO (errorRate);

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

  std::string animFile = "halteres-"+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+std::to_string(dataRate)+".xml";

  // Create the point-to-point link helpers
  PointToPointHelper pointToPointRouter;
  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("10Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("1ms"));
  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("10Mbps"));
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("1ms"));

  PointToPointDumbbellHelper d (nLeftLeaf, pointToPointLeaf,
                                nRightLeaf, pointToPointLeaf,
                                pointToPointRouter);

  // Install Stack
  InternetStackHelper stack;
  d.InstallStack (stack);

  // Assign IP Addresses
  d.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),
                         Ipv4AddressHelper ("10.3.1.0", "255.255.255.0"));

  // Install on/off app on all right side nodes
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
  clientHelper.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable"));
  clientHelper.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable"));
  ApplicationContainer clientApps;

  for (uint32_t i = 0; i < ((d.RightCount () < d.LeftCount ()) ? d.RightCount () : d.LeftCount ()); ++i)
    {
      // Create an on/off app sending packets to the same leaf right side
      AddressValue remoteAddress (InetSocketAddress (d.GetLeftIpv4Address (i), 1000));
      clientHelper.SetAttribute ("Remote", remoteAddress);
      clientApps.Add (clientHelper.Install (d.GetRight (i)));
    }

  clientApps.Start (Seconds (0.0));
  clientApps.Stop (Seconds (10.0));

  // Set the bounding box for animation
  d.BoundingBox (1, 1, 100, 100);

  // Create the animation object and configure for specified output
  AnimationInterface anim (animFile);
  anim.EnablePacketMetadata (); // Optional
  anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (10)); // Optional

  // Set up the actual simulation
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  pointToPointRouter.EnablePcapAll ("halteres");

  AsciiTraceHelper ascii;

  pointToPointRouter.EnableAsciiAll (ascii.CreateFileStream ("halteres-"+typeTCP+"-"+nomeTCP+"-"+std::to_string(errorRate)+"-"+std::to_string(dataRate)+".tr"));
  NS_LOG_INFO ("Rodando a simulação.");


  Simulator::Run ();
  std::cout << "Animation Trace file created:" << animFile.c_str ()<< std::endl;
  Simulator::Destroy ();
  return 0;
}
