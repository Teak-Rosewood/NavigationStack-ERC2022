#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/io/pcd_io.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>

#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>

#include <pcl/features/normal_3d.h>

#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>

class segmentation
{
public:
    ros::Publisher pub;

    // Constructor

    segmentation(std::string sub_topic, std::string pub_topic, ros::NodeHandle n)
    {
        // Initializing Publishers and Subscribers

        pub = n.advertise<sensor_msgs::PointCloud2>(pub_topic, 1);
    }

    // Callback Function

    void cloud_callback(const sensor_msgs::PointCloud2ConstPtr &PointCloud2);

private:
    // Preprocessing of data
    void preprocessing(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_processing);
    // Ground Plane Elimination
    void ground_plane_elimination(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_processing);
    // Clustering
    void clustering(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_processing);
    sensor_msgs::PointCloud2 data;
};

void segmentation::cloud_callback(const sensor_msgs::PointCloud2ConstPtr &PointCloud2)
{

    // PCL and PointCloud2 Point cloud

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PCLPointCloud2 cloud_pcl2;
    sensor_msgs::PointCloud2 pointcloud2;

    // Converting PointCloud2 to PCL Format

    pcl_conversions::toPCL(*PointCloud2, cloud_pcl2);
    pcl::fromPCLPointCloud2(cloud_pcl2, *cloud);
    preprocessing(cloud);
    ground_plane_elimination(cloud);
    pcl::toROSMsg(*cloud, data);
    data.header.frame_id = "camera_optical_frame";

    pub.publish(data);
}

void segmentation::preprocessing(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_processing)
{
    // Pass through filter
    
    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(cloud_processing);
    pass.setFilterFieldName("z");
    pass.setFilterLimits(0.5, 3);
    pass.filter(*cloud_processing);

    pcl::PassThrough<pcl::PointXYZ> pass2;
    pass2.setInputCloud(cloud_processing);
    pass2.setFilterFieldName("x");
    pass2.setFilterLimits(-1.5,1.5);
    pass2.setFilterLimitsNegative (false);
    pass2.filter(*cloud_processing);
    /*
    pcl::PassThrough<pcl::PointXYZ> pass2;
    pass2.setInputCloud(cloud_processing);
    pass2.setFilterFieldName("y");
    pass2.setFilterLimits(0.01, 10);
    pass2.setFilterLimitsNegative (true);
    pass2.filter(*cloud_processing);

    /*
    pcl::PassThrough<pcl::PointXYZRGB> pass3;
    pass3.setInputCloud(cloud_processing);
    pass3.setFilterFieldName("x");
    pass3.setFilterLimits(3,100000);
    
    pass3.filter(*cloud_processing);
    */
    // Voxelization.

    pcl::PCLPointCloud2::Ptr voxelization(new pcl::PCLPointCloud2());
    pcl::toPCLPointCloud2(*cloud_processing, *voxelization);

    pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
    sor.setInputCloud(voxelization);
    sor.setLeafSize(0.05f, 0.05f, 0.05f);
    sor.filter(*voxelization);
    pcl::fromPCLPointCloud2(*voxelization, *cloud_processing);
}

void segmentation::ground_plane_elimination(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_processing)
{
    // // Normal Estimation (NOT IN USE CURRENTLY)

    // pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> ne;
    // ne.setInputCloud (cloud_processing);

    // pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGB> ());
    // ne.setSearchMethod (tree);

    // // Output datasets
    // pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);

    // ne.setRadiusSearch (0.04);

    // // Compute the features
    // ne.compute (*cloud_normals);

    // Plane segmentation using RANSAC
    

    pcl::PointIndices::Ptr indices (new pcl::PointIndices ());
    std::vector<int> inliers;

    pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr
    model_p (new pcl::SampleConsensusModelPlane<pcl::PointXYZ> (cloud_processing));

    pcl::RandomSampleConsensus<pcl::PointXYZ> ransac (model_p);
    ransac.setDistanceThreshold (0.4);
    ransac.computeModel();
    ransac.getInliers(inliers);

    indices->indices = inliers;

    pcl::ExtractIndices<pcl::PointXYZ> extract;
    extract.setInputCloud(cloud_processing);
    extract.setIndices(indices);
    extract.setNegative(true);
    extract.filter(*cloud_processing);


   /*
    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
    // Create the segmentation object
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    // Optional
    seg.setOptimizeCoefficients (true);
    // Mandatory
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setDistanceThreshold (0.1);

    seg.setInputCloud (cloud_processing);
    seg.segment (*inliers, *coefficients);
    */
    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(cloud_processing);
    pass.setFilterFieldName("z");
    pass.setFilterLimits(0.5, 3.0);
    pass.filter(*cloud_processing);
    
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    ne.setInputCloud (cloud_processing);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
    ne.setSearchMethod (tree);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);
    ne.setRadiusSearch (0.5);
    ne.compute (*cloud_normals);
    
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree2 (new pcl::search::KdTree<pcl::PointXYZ>);
    tree2->setInputCloud(cloud_processing);

    std::vector<pcl::PointIndices> cluster_indices;
    pcl::EuclideanClusterExtraction<pcl::PointXYZ> Euclidean_cluster;
    Euclidean_cluster.setClusterTolerance (0.01); // 2cm
    Euclidean_cluster.setMinClusterSize (50);
    Euclidean_cluster.setMaxClusterSize (20000);
    Euclidean_cluster.setSearchMethod (tree2);
    Euclidean_cluster.setInputCloud(cloud_processing);
    Euclidean_cluster.extract(cluster_indices);

    for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
    {
      pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZ>);
      for (const auto& idx : it->indices){
        cloud_cluster->push_back ((*cloud_processing)[idx]); //*
      }
       cloud_cluster->width = cloud_cluster->size ();
       cloud_cluster->height = 1;
       cloud_cluster->is_dense = true;
       *cloud_processing += *cloud_cluster;
     }
    pcl::PassThrough<pcl::PointXYZ> pass3;
    pass3.setInputCloud(cloud_processing);
    pass3.setFilterFieldName("z");
    pass3.setFilterLimits(0.5, 3);
    pass3.filter(*cloud_processing);

    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    sor.setInputCloud (cloud_processing);
    sor.setMeanK (50);
    sor.setStddevMulThresh (1.0);
    sor.filter (*cloud_processing);
    
}


int main(int argc, char **argv)
{
    // Initializing ROS Node

    ros::init(argc, argv, "pcl_node");
    ros::NodeHandle n;
    ros::Rate rate(10);

    std::string sub_topic = "/zed2/point_cloud/cloud_registered";
    std::string pub_topic = "/pcl_cloud";

    segmentation Segmentation(sub_topic, pub_topic, n);

    ros::Subscriber sub = n.subscribe(sub_topic, 1, &segmentation::cloud_callback, &Segmentation);

    rate.sleep();

    // Initiating ROS Spin

    ros::spin();
}