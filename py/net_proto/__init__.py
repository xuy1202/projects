#coding: utf-8

'''
                         |                      |                   |
       'LINK LAYER'      |   'NETWORK LAYER'    | 'TRANSPORT LAYER' |   'APPLICATION LAYER'
                         |                      |                   |

   |-------------------------> IPv6/PPPP/MPLS
   |         |
ETHERNET -> VLAN -> ARP
   |         |
   |-------------------------> IP --> ICMP
                               |
                               |----> IGMP
                               |
                               |-----------------------> UDP
                               |
                               |-----------------------> TCP


'''



