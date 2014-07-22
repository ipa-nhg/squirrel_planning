#include "SceneDatabase.h"

namespace KCL_rosplan {

	void SceneDatabase::addPointCloud(const perception_msgs::SegmentedObject::ConstPtr& msg) {
		clouds[msg->name] = msg->segment;
	}

	void SceneDatabase::removePointCloud(const std_msgs::String::ConstPtr& msg) {
		std::map<std::string, sensor_msgs::PointCloud2>::iterator mit;
		mit = clouds.find(msg->data);
		if(mit!=clouds.end()) {
			clouds.erase(mit);
		}
	}

	bool SceneDatabase::getPointCloud(
			planning_knowledge_msgs::PointCloudService::Request  &req,
			planning_knowledge_msgs::PointCloudService::Response &res) {
		res.cloud = clouds[req.name];
		return true;
	}

	bool SceneDatabase::getPosition(planning_knowledge_msgs::PositionService::Request  &req, planning_knowledge_msgs::PositionService::Response &res) {
		res.position = positions[req.name];
		return true;
	}
	
	void SceneDatabase::addPosition(const perception_msgs::ObjectPosition::ConstPtr& msg) {
		positions[msg->name] = msg->position;
	}

	void SceneDatabase::removePosition(const std_msgs::String::ConstPtr& msg) {
		std::map<std::string, geometry_msgs::Point>::iterator mit;
		mit = positions.find(msg->data);
		if(mit!=positions.end()) {
			positions.erase(mit);
		}
	}
}

	/*-------------*/
	/* Main method */
	/*-------------*/

	int main(int argc, char **argv) {

		ros::init(argc,argv,"squirrel_scene_database");
		ros::NodeHandle nh("~");

		KCL_rosplan::SceneDatabase sd;
		
		ros::ServiceServer pointCloudServer = nh.advertiseService("/kcl_rosplan/get_point_cloud",
			&KCL_rosplan::SceneDatabase::getPointCloud, &sd);
		ros::Subscriber addSub = nh.subscribe("/kcl_rosplan/add_point_cloud", 1000,
			&KCL_rosplan::SceneDatabase::addPointCloud, &sd);
		ros::Subscriber removeSub = nh.subscribe("/kcl_rosplan/remove_point_cloud", 1000,
			&KCL_rosplan::SceneDatabase::removePointCloud, &sd);

		ros::ServiceServer positionServer = nh.advertiseService("/kcl_rosplan/get_object_position",
			&KCL_rosplan::SceneDatabase::getPosition, &sd);
		ros::Subscriber addPositionSub = nh.subscribe("/kcl_rosplan/add_object_position", 1000,
			&KCL_rosplan::SceneDatabase::addPosition, &sd);
		ros::Subscriber removePositionSub = nh.subscribe("/kcl_rosplan/remove_object_position", 1000,
			&KCL_rosplan::SceneDatabase::removePosition, &sd);
		
		ros::spin();
		return 0;
	}