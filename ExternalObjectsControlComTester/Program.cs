using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ExternalObjectsControlComLib;
namespace ExternalObjectsControlComTester
{
    class Program
    {
        static void Main(string[] args)
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
    }
}
