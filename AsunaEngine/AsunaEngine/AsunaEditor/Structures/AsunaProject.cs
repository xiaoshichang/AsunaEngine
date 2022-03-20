using System.IO;
using Newtonsoft.Json;
using System.Runtime.Serialization;
using System.Text;

namespace AsunaEditor.Structures
{
    public enum AsunaProjectCreateResult
    {
        Success = 0,
        AllreadyExist = 1
    }

    public enum AsunaProjectLoadResult
    {
        Success = 0
    }

    [DataContract]
    public class AsunaProject
    {
        [DataMember]
        public string ProjectRootPath { get; set; }

        [DataMember]
        public string ProjectVersion { get; set; }

        public void Save()
        {
            var projectFilePath = Path.Combine(ProjectRootPath, ProjectFileName);
            var ProjectFile = File.Open(projectFilePath, FileMode.OpenOrCreate);
            ProjectFile.Seek(0, SeekOrigin.Begin);
            var content = JsonConvert.SerializeObject(this, Formatting.Indented);
            var data = Encoding.Default.GetBytes(content);
            ProjectFile.Write(data, 0, data.Length);
        }

        public static AsunaProjectLoadResult Load(string projectFile)
        {
            var content = File.ReadAllText(projectFile);
            var project = JsonConvert.DeserializeObject<AsunaProject>(content);
            Current = project;
            return AsunaProjectLoadResult.Success;
        }

        public static AsunaProjectCreateResult Create(string projectRootPath, string projectVersion)
        {
            var project = new AsunaProject();
            var projectFilePath = Path.Combine(projectRootPath, ProjectFileName);

            if (File.Exists(projectFilePath))
            {
                return AsunaProjectCreateResult.AllreadyExist;
            }

            project.ProjectRootPath = projectRootPath;
            project.ProjectVersion = projectVersion;
            Current = project;

            project.Save();
            return AsunaProjectCreateResult.Success;
        }

        public const string ProjectFileName = "AsunaProject.json";
        public static AsunaProject Current = null;

    }

    
}
