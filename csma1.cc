#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/netanim-module.h"

#include "ns3/netanim-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  	bool verbose = true;
  	uint32_t nCsma = 5;

  	CommandLine cmd;
  	cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  	cmd.Parse (argc,argv);

  	if (verbose)
    	{
      	LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
      	LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);
    	}

  	nCsma = nCsma == 0 ? 1 : nCsma;
  	NodeContainer csmaNodes;
  	csmaNodes.Create (nCsma);
  	CsmaHelper csma;
  	csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  	csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
		
  	NetDeviceContainer csmaDevices;
  	csmaDevices = csma.Install (csmaNodes);
  	//Stationnode container      
  	NodeContainer stationNode1;
  	stationNode1.Create (2);
  	NodeContainer stationNode2;
  	stationNode2.Create (2);
  	NodeContainer stationNode3;
  	stationNode3.Create (2);
  	NodeContainer stationNode4;
 	stationNode4.Create (2);


 	//WifiApnodes Container
  	NodeContainer wifiApNode1 = csmaNodes.Get (1);
  	NodeContainer wifiApNode2 = csmaNodes.Get (2);
  	NodeContainer wifiApNode3 = csmaNodes.Get (3);
  	NodeContainer wifiApNode4 = csmaNodes.Get (4);
  	YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  	YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  	phy.SetChannel (channel.Create ());
  
  	//WifiHelper
  	WifiHelper wifi1;
	wifi1.SetRemoteStationManager ("ns3::MinstrelWifiManager", "RtsCtsThreshold", UintegerValue (999999));
  	wifi1.SetRemoteStationManager ("ns3::AarfWifiManager");
	WifiHelper wifi2;
  	wifi2.SetRemoteStationManager ("ns3::AarfWifiManager");
	WifiHelper wifi3;
  	wifi3.SetRemoteStationManager ("ns3::AarfWifiManager");
	WifiHelper wifi4;
  	wifi4.SetRemoteStationManager ("ns3::AarfWifiManager");

 	//WifiMacHelper
  	WifiMacHelper mac1;
  	WifiMacHelper mac2;
  	WifiMacHelper mac3;
  	WifiMacHelper mac4;

  	Ssid ssid1 = Ssid ("ns-3-ssid");

	mac1.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid1),
               "ActiveProbing", BooleanValue (true));
	phy.Set ("TxPowerStart", DoubleValue (20.0));
	phy.Set ("TxPowerEnd", DoubleValue (20.0));


	mac2.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid1),
               "ActiveProbing", BooleanValue (true));
	mac3.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid1),
               "ActiveProbing", BooleanValue (true));
	mac4.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid1),
               "ActiveProbing", BooleanValue (true));




	NetDeviceContainer staDevices1;
	staDevices1 = wifi1.Install (phy, mac1, stationNode1);

	mac1.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid1));
	mac2.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid1));
	mac3.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid1));
	mac4.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid1));

  	//NetDevice Container
  	NetDeviceContainer apDevices1;
  	apDevices1 = wifi1.Install (phy, mac1, wifiApNode1);

  	NetDeviceContainer staDevices2;
  	staDevices2 = wifi2.Install (phy, mac2, stationNode2);

  	NetDeviceContainer apDevices2;
  	apDevices2 = wifi2.Install (phy, mac2, wifiApNode2);

  	NetDeviceContainer staDevices3;
  	staDevices3 = wifi3.Install (phy, mac3, stationNode3);

  	NetDeviceContainer apDevices3;
  	apDevices3 = wifi3.Install (phy, mac3, wifiApNode3);

  	NetDeviceContainer staDevices4;
  	staDevices4 = wifi4.Install (phy, mac4, stationNode4);

  	NetDeviceContainer apDevices4;
  	apDevices4 = wifi4.Install (phy, mac4, wifiApNode4);

  	//stack install
  	InternetStackHelper stack;
  	stack.Install (csmaNodes);
        stack.Install (stationNode4);
        stack.Install (stationNode3);
        stack.Install (stationNode2);
        stack.Install (stationNode1);
  	Ipv4AddressHelper address;
  	address.SetBase ("10.1.1.0", "255.255.255.0");

  	Ipv4InterfaceContainer csmaInterfaces;
  	csmaInterfaces = address.Assign (staDevices1);
  	address.Assign (staDevices2);
  	address.Assign (csmaDevices);
  

  	MobilityHelper mobility;

  	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

	mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
	
  	mobility.Install (stationNode1);
	
  

        Ptr<ConstantVelocityMobilityModel> mobile;
	mobile = (stationNode1.Get(0))->GetObject<ConstantVelocityMobilityModel>();
	mobile->SetPosition(Vector(0.0,125.0,0.0));
	mobile->SetVelocity(Vector(30.0,0.0,0.0));

	mobile = (stationNode1.Get(1))->GetObject<ConstantVelocityMobilityModel>();
	mobile->SetPosition(Vector(0.0,110.0,0.0));
	mobile->SetVelocity(Vector(30.0,0.0,0.0));

        //mobility install
  	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  	mobility.Install (wifiApNode1);
  	mobility.Install (stationNode2);
  	mobility.Install (wifiApNode2);
  	mobility.Install (stationNode3);
  	mobility.Install (wifiApNode3);
  	mobility.Install (stationNode4);
  	mobility.Install (wifiApNode4);
 

 	UdpServerHelper srv(9);  
  	ApplicationContainer srv_apps = srv.Install (stationNode1.Get(0)); 
  	srv_apps.Start (Seconds (0.5));
  	srv_apps.Stop (Seconds (20.0));

  	UdpClientHelper client(csmaInterfaces.GetAddress (0), 9); 
  	client.SetAttribute("MaxPackets",UintegerValue (64707202));
  	client.SetAttribute("Interval",TimeValue (Time ("0.01"))); 
  	client.SetAttribute("PacketSize",UintegerValue (1450));    
  	ApplicationContainer cln_apps = client.Install (csmaNodes.Get(0)); 
  	cln_apps.Start (Seconds (0.5));
  	cln_apps.Stop (Seconds (20.0));
  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  	AnimationInterface anim("HANDOFF-csma-shallini.xml");
	//Update wifiApNode color
        anim.UpdateNodeColor(wifiApNode1.Get(0),255,255,0);
	anim.UpdateNodeColor(wifiApNode2.Get(0),255,255,0);
        anim.UpdateNodeColor(wifiApNode3.Get(0),255,255,0);
        anim.UpdateNodeColor(wifiApNode4.Get(0),255,255,0);
	
	//Update station nodecolor
	anim.UpdateNodeColor(stationNode1.Get(0),0,255,255);
	anim.UpdateNodeColor(stationNode1.Get(1),0,255,255);
	anim.UpdateNodeColor(stationNode2.Get(0),0,255,255);
	anim.UpdateNodeColor(stationNode2.Get(1),0,255,255);
	anim.UpdateNodeColor(stationNode3.Get(0),0,255,255);
	anim.UpdateNodeColor(stationNode3.Get(1),0,255,255);
	anim.UpdateNodeColor(stationNode4.Get(0),0,255,255);
	anim.UpdateNodeColor(stationNode4.Get(1),0,255,255);
	anim.UpdateNodeColor(stationNode1.Get(1),0,255,255);

	//UpdateWifiApNode description
	anim.UpdateNodeDescription(wifiApNode1.Get(0),"Ap-1");
        anim.UpdateNodeDescription(wifiApNode2.Get(0),"Ap-2");
        anim.UpdateNodeDescription(wifiApNode3.Get(0),"Ap-3");
        anim.UpdateNodeDescription(wifiApNode4.Get(0),"Ap-4");

	//UpdateNodeDescription
	anim.UpdateNodeDescription(stationNode2.Get(0),"Station-2-1 node");
	anim.UpdateNodeDescription(stationNode2.Get(1),"Station-2-2 node");
	anim.UpdateNodeDescription(stationNode1.Get(0),"Station-1-1 node");
	anim.UpdateNodeDescription(stationNode1.Get(1),"Station-1-2 node");
	anim.UpdateNodeDescription(stationNode3.Get(1),"Station-3-1 node");
	anim.UpdateNodeDescription(stationNode3.Get(1),"Station-3-2 node");
	anim.UpdateNodeDescription(stationNode4.Get(1),"Station-4-1 node");
	anim.UpdateNodeDescription(stationNode4.Get(1),"Station-4-2 node");

	
        anim.UpdateNodeDescription(csmaNodes.Get (0),"Server Node");
	anim.SetConstantPosition(csmaNodes.Get (0),100.0,50.0);

	//ConstantPsition WifiApNodes
        anim.SetConstantPosition(wifiApNode1.Get(0),0.0,100.0);
	anim.SetConstantPosition(wifiApNode2.Get(0),100.0,100.0);
	anim.SetConstantPosition(wifiApNode3.Get(0),200.0,100.0);
	anim.SetConstantPosition(wifiApNode4.Get(0),300.0,100.0);

        //StationNode ConstantPosition
	anim.SetConstantPosition(stationNode2.Get(0),75.0,150.0);
	anim.SetConstantPosition(stationNode2.Get(1),125.0,150.0);
	anim.SetConstantPosition(stationNode3.Get(1),225.0,150.0);
	anim.SetConstantPosition(stationNode3.Get(0),175.0,150.0);
	anim.SetConstantPosition(stationNode4.Get(0),275.0,150.0);
	anim.SetConstantPosition(stationNode4.Get(1),315.0,150.0);

	
	Simulator::Stop(Seconds(20.0));
  	Simulator::Run ();
  	Simulator::Destroy ();
  	return 0;
}
