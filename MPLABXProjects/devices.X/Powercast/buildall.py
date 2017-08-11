#!/usr/bin/python

import argparse
import re
import subprocess
import select

from beeprint import pp
import pdb

# This is the list of radio regions that are to be supported.
#
# NA - North_America
# UK - United_Kingdom
#
# From the perspective of this file, the radio region suffix is
# appended to the component name to produce the component specialized
# to that radio region.

radio_regions = ["NA", "UK"]

# This table defines the devices and configurations that are supported
# by the device build system.  It is business logic that models the
# actual structure and contents of the gitlab devices project.

configurations = [
    { 'name': "Certification",
      'build_dir': "Certification",
      'config_list': ["BatteryDevice", "Connector"]},
    { 'name': "LightSensor",
      'build_dir': "LightSensor/LightSensor.X",
      'config_list': ["Production"]},
    { 'name': "OccupancySensor",
      'build_dir': "OccupancySensor/OccupancySensor.X",
      'config_list': ["Production"]},
    { 'name': "OccupancySensor-LowTemp",
      'build_dir': "OccupancySensor-LowTemp/OccupancySensor-LowTemp.X",
      'config_list': ["Production"]},
    { 'name': "OccupancySensor-Ultrasonic",
      'build_dir': "OccupancySensor-Ultrasonic/UltrasonicSensor.X",
      'config_list': ["Production"]},
    { 'name': "PartitionSensor",
      'build_dir': "PartitionSensor/PartitionSensor.X",
      'config_list': ["Production"] },
    { 'name': "SmartConnector",
      'build_dir':"SmartConnector/SmartConnector.X",
      'config_list': ["Production" ]},
    { 'name': "SmartSwitch",
      'build_dir': "SmartSwitch/SmartSwitch.X",
      'config_list': [ "OnOffFlush",
                     "RegularFlush",
                     "RegularWall",
                     "SceneFlush",
                     "SceneWall" ]}]

# This function represents the file names of the various build
# products.  It is business logic that models the relevant portions of
# the file naming conventions standards document that defines the
# names of all of the deliverables for the Audacy Lighting Control
# System.

def get_firmware_name(comp_name,config_name,radio_region,git_commit):

    if radio_region not in [ "NA", "UK" ]:
        raise NotImplementedError

    if comp_name == "Certification":
        if config_name == "BatteryDevice":
            return "audacy-certification-battery-" + radio_region + ".hex"
        elif config_name == "Connector":
            return "audacy-certification-line-" + radio_region + ".hex"
        else:
            raise NotImplementedError
    elif comp_name == "LightSensor":
        if config_name != "Production":
            raise NotImplementedError
        return "audacy-device-07-" + radio_region + ".hex"
    elif comp_name == "OccupancySensor":
        if config_name != "Production":
            raise NotImplementedError
        return "audacy-device-05" + radio_region + ".hex"
    elif comp_name == "OccupancySensor-LowTemp":
        if config_name != "Production":
            raise NotImplementedError
        return "audacy-device-19-" + radio_region + ".hex"
    elif comp_name == "OccupancySensor-Ultrasonic":
        if config_name != "Production":
            raise NotImplementedError
        return "audacy-device-14-" + radio_region + ".hex"
    elif comp_name == 'PartitionSensor':
        if config_name != "Production":
            raise NotImplementedError
        return "audacy-device-15-" + radio_region + ".hex"
    elif comp_name == "SmartConnector":
        if config_name != "Production":
            raise NotImplementedError
        return git_commit + radio_region + ".hex"
    elif comp_name == "SmartSwitch":
        if config_name == "OnOffFlush":
            return "audacy-device-0f-" + radio_region + ".hex"
        elif config_name == "RegularWall":
            return "audacy-device-04-" + radio_region + ".hex"
        elif config_name == "RegularFlush":
            return "audacy-device-0a-" + radio_region + ".hex"
        elif config_name == "SceneWall":
            return "audacy-device-0d-" + radio_region + ".hex"
        elif config_name == "SceneFlush":
            return "audacy-device-0c-" + radio_region + ".hex"
    else:
        raise NotImplementedError

class BuildProcess(object):

    def create_process(self):
        self.cmd = subprocess.Popen(['bash'],
                                    stdin=subprocess.PIPE,
                                    stdout=subprocess.PIPE)
        self.poll = select.poll()
        self.poll.register(self.cmd.stdout.fileno(),select.POLLIN)

    def _execute_command(self,command):
        self.cmd.stdin.write(command + "\n")
        self.cmd.stdin.flush()
        ready = self.poll.poll(500)
        while ready:
            result = self.cmd.stdout.readline()
            print result,
            ready = self.poll.poll(500)

    def terminate_process(self):
        # This is simplistic.  See the Python subprocess management
        # documentation and read about the wait() and communicate()
        # methods.  If the child process is still generating otuput,
        # then this could deadlock. This shouldn't be a problem for
        # this application as the _execute_command() method waits for
        # the child to complete its output for each command.  There
        # should be no pending output when terminate_process() is
        # called.
        self.cmd.terminate()
        self.cmd.wait()

    def push_directory(self,directory_name):
        self._execute_command("pushd " + directory_name)

    def pop_directory(self):
        self._execute_command("popd")

    def list_current_directory(self):
        self._execute_command("ls ")

    def clean_area(self):
        self._execute_command("make clobber");

    def build_configuration(self,config_name):
        self._execute_command("make CONF=" + config_name)

    def get_git_commit(self):
        self.cmd.stdin.write("git log -1\n")
        self.cmd.stdin.flush()
        commit_line = None
        ready = self.poll.poll(500)
        repattern = "commit[\W]*([a-fA-F0-9]*)"
        commit_pattern = re.compile(repattern)
        while ready:
            result = self.cmd.stdout.readline()
            if commit_pattern.match(result):
                commit_line = result
            ready = self.poll.poll(500)

        if commit_line is None:
            return None

        match_obj = commit_pattern.match(commit_line)
        return match_obj.group(1)

    def check_for_file_existance(self):
        pass

# Operations On An Individual Configuration
#------------------------------------------

class BuildConfiguration(object):

    def __init__(self,
                 makefile_target,
                 target_file_name):
        self._mf_target = makefile_target
        self._target_file = target_file_name

#    def __repr__(self):
#        return "BuildConfiguration(_mf_target=%s, _target_file=%s)" % (self._mf_target,
#                                                                       self._target_file)

    def clean(self,build_proc):
        build_proc.clean_area()

    def build(self,build_proc):
        build_proc.build_configuration(self._mf_target)

    def copy_result_to_target(self,target_dir):
        pass

#Operations On A DeviceComponent
#----------------------
#
#connect_to_build_directory
#construct_all_components

class DeviceComponent(object):

    comp_dict = dict()

    @classmethod
    def _insert_into_dict(cls,build_comp):
        cls.comp_dict[build_comp.get_name()] = build_comp

    @classmethod
    def find_comp_by_name(cls,name):
        return cls.comp_dict[name]

    def __init__(self,name,build_dir,config_list,git_revision):
        self._name = name
        self._build_dir = build_dir
        self._config_names = config_list
        self._insert_into_dict(self)
        self._configs = dict()
        self._git_rev = git_revision
        for cfg_name in config_list:
            for rr in radio_regions:
                full_cfg_name = cfg_name + "-" + rr
                target_file_name = get_firmware_name(name,cfg_name,rr,git_revision)
                bc = BuildConfiguration(full_cfg_name,target_file_name)
                self._configs[full_cfg_name] = bc

    def get_build_dir(self):
        return self._build_dir

    def get_name(self):
        return self._name

    def clean(self,build_proc):
        build_proc.push_directory(self._build_dir)
        for key,cfg in self._configs.items():
            print "Cleaning %s" % key
            cfg.clean(build_proc)
        build_proc.pop_directory()

    def build(self,build_proc):
        build_proc.push_directory(self._build_dir)
        for key,cfg in self._configs.items():
            print "Building %s" % key
            cfg.build(build_proc)
        build_proc.pop_directory()

#    def __repr__(self):
#        return "DeviceComponent(_name=%s,_build_dir=%s,_config_names=%r,_configs=%r)" % (self._name,
#                                                                                         self._build_dir,
#                                                                                        self._config_names,
#                                                                                        self._configs)

def build_components(git_revision):
    for cfg in configurations:
        bc = DeviceComponent(cfg['name'],
                             cfg['build_dir'],
                             cfg['config_list'],
                             git_revision)


def main_processing(args):
    bp = BuildProcess()
    bp.create_process()
    print("====================================")
    git_rev = bp.get_git_commit()
    print "Git Commit is %r" % git_rev
    print("====================================")

    build_components(git_rev)
    pp(DeviceComponent.comp_dict)

    if args.clean:
        for key,value in DeviceComponent.comp_dict.items():
            print "Cleaning Device %s" % key
            value.clean(bp)
    else:
        for key,value in DeviceComponent.comp_dict.items():
            print "Building Device %s" % key
            value.build(bp)

    bp.terminate_process()

#Option Arguments
#----------------
#
# revision - Device Revision Code
# final_directory - Directory in which to place the hex files

def get_args():

    parser = argparse.ArgumentParser(description="Construct All Device Firmware Images")
    parser.add_argument("--verbose",
                        help="Verbose Output",
                        action="store_true")
    parser.add_argument("--clean",
                        help="Clean Build Area",
                        action="store_true")
    parser.add_argument("revision",
                        help="Revision Number",
                        action="store")
    parser.add_argument("final_directory",
                        help="Final Image Directory",
                        action="store")

    args = parser.parse_args()
    return args

args = get_args()
main_processing(args)
