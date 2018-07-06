using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ExternalObjectsControlComLib;
using System.Diagnostics;

namespace ExternalObjectsControlComTester
{
    class Program
    {
        static void testCreateRelease()
        {
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl(1, 0);
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void testInitializeUnInitialize()
        {
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl(1, 0);
                string scenePath = "E:\\Distri_NadsMiniSim_2.2.1\\data\\distri_demo_fhwa.scn";
                pCtrl.Initialize(scenePath);
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void Main(string[] args)
        {
            testCreateRelease();
            testInitializeUnInitialize();

        }
    }
}
