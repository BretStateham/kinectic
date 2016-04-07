using Microsoft.Kinect;
using Microsoft.Kinect.Wpf.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Kinectic
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    #region Fields

    KinectSensor sensor;

    #endregion Fields

    public MainWindow()
    {
      InitializeComponent();

      InitKinectRegion();
    }

    private void InitKinectRegion()
    {
      //Give the UserViewer a way to find the KinectRegion
      KinectRegion.SetKinectRegion(this, kinectRegion);

      //Wire up the current connect region to the App level KinectRegion
      App app = ((App)Application.Current);
      app.KinectRegion = kinectRegion;

      //Get the default KinectSensor
      sensor = KinectSensor.GetDefault();

      //Assign it to the KinectRegion
      this.kinectRegion.KinectSensor = sensor;

      //And open it
      sensor.Open();
    }
  }
}
