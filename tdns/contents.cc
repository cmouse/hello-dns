#include "dns-storage.hh"
#include "record-types.hh"

void loadZones(DNSNode& zones)
{
  auto zone = zones.add({"tdns", "powerdns", "org"});
  auto newzone = zone->zone = new DNSNode(); // XXX ICK
  
  newzone->addRRs(SOAGen::make({"ns1", "tdns", "powerdns", "org"}, {"admin", "powerdns", "org"}, 1),
                  NSGen::make({"ns1", "tdns", "powerdns", "org"}), 
                  MXGen::make(25, {"server1", "tdns", "powerdns", "org"})
                  );
  newzone->add({"server1"})->addRRs(AGen::make("213.244.168.210"), AAAAGen::make("::1"));
  
  newzone->addRRs(AGen::make("1.2.3.4"));
  newzone->addRRs(AAAAGen::make("::1"));
  newzone->rrsets[DNSType::AAAA].ttl= 900;

  newzone->addRRs(TXTGen::make("Proudly served by tdns compiled on " __DATE__ " " __TIME__),
                  TXTGen::make("This is some more filler to make this packet exceed 512 bytes"));
  
  newzone->add({"www"})->rrsets[DNSType::CNAME].add(CNAMEGen::make({"server1","tdns","powerdns","org"}));
  newzone->add({"www2"})->rrsets[DNSType::CNAME].add(CNAMEGen::make({"nosuchserver1","tdns","powerdns","org"}));


  newzone->add({"server2"})->addRRs(AGen::make("213.244.168.210"), AAAAGen::make("::1"));
  
  newzone->add({"*", "nl"})->rrsets[DNSType::A].add(AGen::make("5.6.7.8"));
  newzone->add({"*", "fr"})->rrsets[DNSType::CNAME].add(CNAMEGen::make({"server2", "tdns", "powerdns", "org"}));

  newzone->add({"fra"})->addRRs(NSGen::make({"ns1","fra","powerdns","org"}), NSGen::make({"ns1","fra","powerdns","org"}));
  newzone->add({"ns1"})->addRRs(AGen::make("52.56.155.186"));
  newzone->add({"ns1", "fra"})->addRRs(AGen::make("12.13.14.15"));
  newzone->add({"NS2", "fra"})->addRRs(AGen::make("12.13.14.16"));
  newzone->add({"ns2", "fra"})->addRRs(AAAAGen::make("::1"));  

  newzone->add({"something"})->addRRs(AAAAGen::make("::1"), AGen::make("12.13.14.15"));
  newzone->add({"time"})->addRRs(ClockTXTGen::make("The time is %a, %d %b %Y %T %z"));

  newzone->add({"ent", "was", "here"})->addRRs(TXTGen::make("plenum"));
  newzone->add({"some.embedded.dots"})->addRRs(TXTGen::make("what do the dots look like?"));


  const char zero[]="name-does-not-stop-here\x0-it-goes-on";
  std::string zstring(zero, sizeof(zero)-1);
  newzone->add({"goes-via-embedded-nul"})->addRRs(CNAMEGen::make({zstring, "tdns", "powerdns", "org"}));
  newzone->add({"goes-via-embedded-space"})->addRRs(CNAMEGen::make({"some host", "tdns", "powerdns", "org"}));
  newzone->add({"goes-via-embedded-dot"})->addRRs(CNAMEGen::make({"some.host", "tdns", "powerdns", "org"}));

                                                  
  newzone->add({zstring})->addRRs(TXTGen::make("this record is called name-does-not-stop-here\\000-it-goes-on"),
                                                            AGen::make("192.0.0.1"));

  newzone->add({"some host"})->addRRs(AGen::make("192.0.0.2"));
  newzone->add({"some.host"})->addRRs(AGen::make("192.0.0.3"));
}
