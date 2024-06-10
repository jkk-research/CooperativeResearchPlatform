from setuptools import find_packages
from setuptools import setup

setup(
    name='crp_msgs',
    version='0.0.0',
    packages=find_packages(
        include=('crp_msgs', 'crp_msgs.*')),
)
