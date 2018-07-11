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
        static string s_sceneTest = "E:\\Distri_NadsMiniSim_2.2.1\\data\\distri_demo_fhwa_unitydev.scn";
        enum IMPLE { IGCOMM = 0, DISVRLINK};
        enum TERMINAL {edo_controller = 0, ado_controller, ped_controller};
        static void testCreateRelease()
        {
            Console.WriteLine("testCreateRelease...");
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl((int)IMPLE.DISVRLINK, (int)TERMINAL.ped_controller);
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void testInitializeUnInitialize()
        {
            Console.WriteLine("testInitializeUnInitialize...");
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl((int)IMPLE.DISVRLINK, (int)TERMINAL.ped_controller);
                pCtrl.Initialize(s_sceneTest);
                pCtrl.UnInitialize();
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void testReceiving()
        {
            Console.WriteLine("testReceiving...");
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl((int)IMPLE.DISVRLINK, (int)TERMINAL.ped_controller);
                pCtrl.Initialize(s_sceneTest);

                bool recieving = true;
                EVT evt = EVT.evtUndefined;
                string[] evtNames = { "create dynobj", "delete dynobj", "undefined" };
                List<int> dynObjs = new List<int>();
                while (recieving)
                {
                    bool empty = true;
                    while (true)
                    {
                        pCtrl.QFrontEvent(out evt, out empty);
                        if (empty)
                            break;
                        Console.WriteLine("\n");
                        Console.WriteLine(evtNames[(int)evt]);
                        switch (evt)
                        {
                            case EVT.crtDyno:
                                {
                                    int id;
                                    string name;
                                    int solId;
                                    double xSize, ySize, zSize;
                                    double xPos, yPos, zPos;
                                    double xTan, yTan, zTan;
                                    double xLat, yLat, zLat;
                                    pCtrl.GetcrtDynoTuple(out id, out name, out solId
                                            , out xSize, out ySize, out zSize
                                            , out xPos, out yPos, out zPos
                                            , out xTan, out yTan, out zTan
                                            , out xLat, out yLat, out zLat);
                                    string strTuple = string.Format("\n\tid:{14}\n\tname:{0}\tsolId:{1}\n\tsize:<{2},{3},{4}>\n\tpos:<{5},{6},{7}>\n\ttan:<{8},{9},{10}>\n\tlat:<{11},{12},{13}>"
                                                                    , name, solId
                                                                    , xSize, ySize, zSize
                                                                    , xPos, yPos, zPos
                                                                    , xTan, yTan, zTan
                                                                    , xLat, yLat, zLat
                                                                    , id);
                                    Console.Write(strTuple);
                                    dynObjs.Add(id);
                                    break;
                                }

                            case EVT.delDyno:
                                {
                                    int id;
                                    pCtrl.GetdelDynoTuple(out id);
                                    string strTuple = string.Format("\n\tid:{0}", id);
                                    Console.Write(strTuple);
                                    dynObjs.Remove(id);
                                    break;
                                }
                        }
                        pCtrl.QPopEvent();
                    }

                    pCtrl.PreUpdateDynamicModels();
                    //inject receiving code
                    foreach (int id_local in dynObjs)
                    {
                        bool received = true;
                        double xPos, yPos, zPos;
                        double xTan, yTan, zTan;
                        double xLat, yLat, zLat;
                        pCtrl.OnGetUpdate(id_local, out received
                                    , out xPos, out yPos, out zPos
                                    , out xTan, out yTan, out zTan
                                    , out xLat, out yLat, out zLat);
                        string strTuple = string.Format("\nreceived = {0}:\n\tpos=[{1},{2},{3}]\n\ttan=[{4},{5},{6}]\n\tlat=[{7},{8},{9}]"
                                                        , received, xPos, yPos, zPos, xTan, yTan, zTan, xLat, yLat, zLat);
                        Console.Write(strTuple);
                    }
                    //inject sending state code
                    pCtrl.PostUpdateDynamicModels();


                    ConsoleKeyInfo key = new ConsoleKeyInfo();
                    if (Console.KeyAvailable)
                        key = Console.ReadKey(true);

                    switch (key.Key)
                    {
                        case ConsoleKey.Q:
                        case ConsoleKey.Escape:
                            recieving = false;
                            break;
                    }
                }
                pCtrl.UnInitialize();
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void Main(string[] args)
        {
            //testCreateRelease();
            //testInitializeUnInitialize();
            testReceiving();
        }
    }
}
